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
 * @brief Various operations on directories
 *
 */

#ifndef _GVN_DIRECTORY_HEAD_
#define _GVN_DIRECTORY_HEAD_

#include <core/logger.hpp>
#include <utils/file.hpp>
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
        class Directory
        {
        private:
            std::string dir;

        public:
            Folder(std::string dir_);

            bool exits();
            bool delete();
            
            bool list(std::vector<Utils::Folder> folders, std::vector<Utils::File> files);
            bool findFile(const std::string &regex_pattern, const bool append = false);

            Directory createChild(const string &dir_name);
            Directory parent();

            /// same as pwd command in linux
            static Directory pwd();
        };
        
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        bool Directory::list(vector<Utils::Directory> &folders, vector<Utils::File> &files)
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
                        files.push_back( File( ent->d_name ) );
                        break;

                    case DT_DIR:
                        // printf ("(%s/)\n", ent->d_name);
                        folders.push_back( Directory( ent->d_name ) );
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
        bool Directory::findFiles(vector<File> &files, const string pattern = "", const bool append = false)
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
        bool Directory::createDirectory(const string &name)
        {
#if defined(INFO_OS_WINDOWS)
            return CreateDirectory(dir.c_str(), NULL) != 0;
#else
            /// @TODO: 0750 is not a good idea
            return mkdir(dir.c_str(), 0750) == 0;
            // return _mkdir(dir) == 0;
#endif
        }

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        bool Directory::exits ()
        {
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
/// @todo Incomplete algorithm
        Directory Directory::parent()
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
        bool Directory::delete()
        {
#if defined(INFO_OS_WINDOWS)
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
                    deleteDirectory(buf);
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

    } // utility
} // grav

#endif // _GVN_DIRECTORY_HEAD_
