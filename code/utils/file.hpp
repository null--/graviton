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
 * @brief Various operations on files
 *
 */

#ifndef _GVN_FILE_HEAD_
#define _GVN_FILE_HEAD_

#include <core/logger.hpp>
#include <utils/folder.hpp>

#include <utils/regex.hpp>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
using namespace std;

#if defined(INFO_OS_WINDOWS)
#include <external/dirent/dirent.h>
#include <direct.h>
#else
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#endif

namespace GraVitoN
{
    namespace Utils
    {   
        class File
        {
        private:
            std::string path;
            
        public:
            File(std::string path_);
            
            size_t size();

            template<class T_File>
            Core::Memory<T_File> load();

            template<class T_File>
            bool save(Core::Memory<T_File> data, bool append = false);

            bool exists();
            bool delete();

            /// get parent directory
            Utils::Directory parent();
        };

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        size_t File::size()
        {
            FILE *file = fopen(file_name.c_str(), "rb");
            fseek (file, 0, SEEK_END);
            size_t _size = ftell(file);
            fclose (file);
            return _size;
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
        template <class T_File>
        Core::Memory<T_File> File::load()
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
        bool exists(const string &path)
        {
            FILE *f = fopen(path.c_str(), "r");
            if(!f)
                return false;
            fclose(f);
        }

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        string getWorkingDirectory()
        {
            char buf[256];
#ifdef INFO_OS_WINDOWS
            _getcwd(buf, 255);
#else
            getcwd(buf, 255);
#endif
            return string( buf );
        }

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/// @todo Incomplete algorithm
        string getRootDirectory(const string &path)
        {
            string root;
            int lpos = path.size() - 1;
            while( lpos > 0 && path[lpos] == '/' || path[lpos] == '\\' ) --lpos;
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

    } // utility
} // grav

#endif // _GVN_FILE_HEAD_
