/**
 * @file
 * 
 * @author  Sina Hatef Matbue ( _null_ ) <sinahatef.cpp@gmail.com>
 *
 * @section License
 * This file is part of GraVitoN.
 *
 * Graviton is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Graviton is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Graviton.  If not, see http://www.gnu.org/licenses/.
 *
 * @brief Various file-based operations
 *
*/

#ifndef _GVN_FILES_HEAD_
#define _GVN_FILES_HEAD_

#include <core/logger.hpp>

#include <utils/regex.hpp>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
using namespace std;

#if defined(INFO_OS_WINDOWS)
    #include <external/dirent/dirent.h>
#else
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <dirent.h>
#endif

namespace GraVitoN
{

namespace Utils
{

/// Various file-based operations
namespace File
{

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
size_t getSize(const string &file_name)
{
    FILE *file = fopen(file_name.c_str(), "rb");
    fseek (file, 0, SEEK_END);
    size_t size = ftell(file);
    fclose (file);
    return size;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/**
 * @brief Load a binary file into an unsigned char array
 * 
 * @param [i] path
 * Path to binary file
 *
 * @param [out] buffer
 * Pointer to your buffer pointer (double pointer). we needs your pointer to buffer pointer
 * because after return of this function c++ will free your allocated buffer memory.
 * see C++ refrence for more information. \n
 * if buffer == NULL then loadFile is failed
 *
 * @return
 * Size of our buffer, or 0 and if it's failed
 *
 */
unsigned long loadFile(const string &path, unsigned char **buffer)
{
	unsigned long size = 0;
	
	if( *buffer != NULL)
	  free(*buffer);
	
	FILE *file;
	
    Core::Logger::logIt("Reading Binary File... ");
	file = fopen(path.c_str(), "rb");
	if( file )
	{
		*buffer = (unsigned char*)malloc(Config::MAX_FILE_SIZE);
		size = 0;
		
		while( !feof(file) )
		{
			unsigned char ch = fgetc(file);
			//printf("%.9d < %.2x\n", size, (*buffer)[size]);
			if(!feof(file))
			{
				*(*buffer + size) = ch;
				size = size + 1;
			}
		}
        Core::Logger::logIt("[Size: "); Core::Logger::logIt(size);
        Core::Logger::logItLn("] DONE");
		
		fclose(file);
	}
	else
	{
		*buffer = NULL;
	}
	return size;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/**
 * @brief Save your buffer as a binary file
 *
 * @param [in] opath
 * Path to output file
 *
 * @param [in] buffer
 * Pointer to your buffer
 *
 * @param [in] size
 * Size of your buffer
 *
 * @return
 * True if buffer was saved to file successfully
 */
bool saveUChars(const string opath, const unsigned char *buffer, const unsigned long &size, const bool append = false)
{
	FILE *outf;
	outf = (append)?fopen(opath.c_str(), "ab+"):fopen(opath.c_str(), "wb+");

	if( !outf )
		return false;
	
    Core::Logger::logIt("Writing to binary file... ");
	for(register unsigned long i = 0; i < size; ++i)
	{
		fputc(*(buffer + i), outf);
		//printf("%.9d > %.2x\n", i, buffer[i]);
	}
    Core::Logger::logItLn("DONE");
	
	fclose(outf);

	return true;

}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/**
 * @brief Save your buffer as a cpp file
 *
 * @param [in] opath
 * Path to output file
 *
 * @param [in] buffer
 * Pointer to your buffer
 *
 * @param [in] size
 * Size of your buffer
 *
 * @return
 * True if buffer was saved to file successfully
 */
bool printUCharsIntoFile(const string opath, const unsigned char *buffer, const unsigned long &size)
{
	FILE *outf;
	outf = fopen(opath.c_str(), "w+");

	if( !outf )
		return false;

	int mod = 0;
	fprintf(outf, "const unsigned long buf_len = %lu;\n"
					"unsigned char buf[] = \n",
					size);
    Core::Logger::logIt("Writing to cpp file... ");
    Core::Logger::logIt("Size: ");
    Core::Logger::logIt("Size: ");
	for(unsigned long i = 0; i < size; ++i)
	{
		//printf(".2x\n", buffer[i]);
		if( mod % 16 == 0 )
		{
			fprintf(outf, "\"");
			mod = 0;
		}
		fprintf(outf, "\\x%.2x", buffer[i]);

		if( mod % 16 == 15 )
		{
			fprintf(outf, "\"\n");
			mod = 0;
		}
		else
		{
			++mod;
		}
	}
	if( (mod > 0) && (mod % 16 != 15) )
	{
		fprintf(outf, "\"");
		mod = 0;
	}
    Core::Logger::logItLn("DONE");

	fprintf(outf, "\n;");
	fclose(outf);

	return true;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool listDirectory(const string &dirname, vector<string> &folders, vector<string> &files)
{
    folders.clear();
    files.clear();
    
    DIR *dir;
    struct dirent *ent;
                
    /* Open directory stream */
    dir = opendir (dirname.c_str());
    if (dir != NULL)
    {

        /* Print all files and directories within the directory */
        while ((ent = readdir (dir)) != NULL) {
            switch (ent->d_type) {
            case DT_REG:
                // printf ("[%s]\n", ent->d_name);
                files.push_back(ent->d_name);
                break;

            case DT_DIR:
                // printf ("(%s/)\n", ent->d_name);
                folders.push_back(ent->d_name);
                break;

                // default:
                // printf ("{%s*}\n", ent->d_name);
            }
        }

        closedir (dir);

    } else
    {
        /* Could not open directory */
        // printf ("Cannot open directory %s\n", dirname);
        return false;
    }
    return true;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/// @todo: Do a BFS
/// append: do not clear files vector
/// pattern: a valid regular expression for search (see utils/regex.hpp for more information)
bool findAllFiles(const string &dirname, vector<string> &files, const string pattern = "", const bool append = false)
{
    // Core::Logger::logVariable("[File] ", dirname);
    
    DIR *dir;
    char buffer[PATH_MAX + 2];
    char *p = buffer;
    const char *src;
    char *end = &buffer[PATH_MAX];

    /* Copy directory name to buffer */
    src = dirname.c_str();
    while (p < end  &&  *src != '\0') {
        *p++ = *src++;
    }
    *p = '\0';

    bool search = !pattern.empty();

    Utils::Regex rex;
    if( search  )
    {
        // cout << "COMILE" << endl;
        if( !rex.compile(pattern) )
        {
            Core::Logger::logItLn("[File] findAllFiles invalid regex pattern");

            // search = false;
            /// 
            return false;
        }
    }
        
    /* Open directory stream */
    dir = opendir (dirname.c_str());
    if (dir != NULL) {
        struct dirent *ent;

        /* Print all files and directories within the directory */
        while ((ent = readdir (dir)) != NULL)
        {
            char *q = p;
            char c;

            /* Get final character of directory name */
            if (buffer < q)
            {
                c = q[-1];
            } else {
                c = ':';
            }

            /* Append directory separator if not already there */
            if (c != ':'  &&  c != '/'  &&  c != '\\')
            {
                *q++ = '/';
            }

            /* Append file name */
            src = ent->d_name;
            while (q < end  &&  *src != '\0')
            {
                *q++ = *src++;
            }
            *q = '\0';

            string bf;
            /* Decide what to do with the directory entry */
            switch (ent->d_type)
            {
            case DT_REG:
                /* Output file name with directory */
                // printf ("%s\n", buffer);
                // Core::Logger::logVariable("[File]", buffer);

                if( search && !rex.match(buffer) )
                {
                    // Core::Logger::logVariable("[File] Not Match", buffer);
                    break;
                }
               

                // Core::Logger::logVariable("[File] Match", buffer);
                
                files.push_back(buffer);
                break;

            case DT_DIR:
                /* Scan sub-directory recursively */
                if (strcmp (ent->d_name, ".") != 0  
                        &&  strcmp (ent->d_name, "..") != 0)
                {
                    findAllFiles (buffer, files, pattern, append);
                }
                break;

            default:
                /* Do not device entries */
                /*NOP*/;
                break;
            }

        }

        closedir (dir);
    }
    else
    {
        /* Could not open directory */
        Core::Logger::logVariable("Cannot open directory ", dirname);
        return false;
    }

    return true;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool createFolder(const string &dir)
{
#if defined(INFO_OS_WINDOW)
    return CreateDirectory(dir.c_str(), NULL) != 0;
#else
    /// @TODO: 0750 is not a good idea
    return mkdir(dir.c_str(), 0750) == 0;
    // return _mkdir(dir) == 0;
#endif
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool isDirectory (const string &path) {
    DIR *dir;
    dir = opendir (path.c_str());

    if( dir )
    {
        closedir(dir);
        return true;
    }
    return false;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool pathExists(const string &path)
{
    if(isDirectory(path))
    {
        return true;
    }
    else
    {
        FILE *f = fopen(path.c_str(), "r");
        if(!f)
            return false;
        fclose(f);
        return true;
    }
    
    return false;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
string getRootDirectory(const string &path)
{
    string root;
    int lpos = path.size() - 1;
    while(lpos > 0 && path[lpos] != '/' && path[lpos] != '\\')
        --lpos;
    for(int i=0; i<lpos; ++i)
        root = root + path[i];
    return root;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool deleteFile(const string &file)
{
    return std::remove(file.c_str()) == 0;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
bool deleteFolder(const string &folder)
{
#if defined(INFO_OS_WINDOW)
    return RemoveDirectory((LPCSTR)folder.c_str()) != 0;
#else
    DIR*            dp;
    struct dirent*  ep;
    string          buf, tmp;

    dp = opendir(folder.c_str());

    if( !dp ) return false;
    while ((ep = readdir(dp)) != NULL)
    {
        tmp = ep->d_name;
        if( tmp == "." || tmp == ".." )
            continue;
        
        buf = folder + "/" + tmp;
        if (isDirectory(buf))
        {
            deleteFolder(buf);
        }
        else
        {
            Core::Logger::logVariable("[FILE] Deleting", buf);
            // continue;
            
            if( !deleteFile(buf) )
            {
                Core::Logger::logVariable("Unable to delete: ", buf);
                return false;
            }
        }
    }

    closedir(dp);
        
    return rmdir(folder.c_str()) == 0;
#endif
}

} // file
} // utility
} // grav

#endif // _GVN_FILES_HEAD_
