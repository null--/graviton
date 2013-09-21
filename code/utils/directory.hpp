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
        protected:
            std::string dirname;

        public:
            Directory(std::string dir_);
            Directory(const File &file_);
            
            bool exists() const;
            Directory parent() const;
            Directory child(const string &child_name) const;
            string getPath() const;
            
            bool list(std::vector<Utils::Directory> &folders, std::vector<Utils::File> &files) const;
            bool findFiles(vector<File> &files, const string pattern = "", const bool append = false) const;

            Directory createSubDirectory(const string &child);
            bool create();
            bool remove();
            
            /// Current Working directory
            static Directory cwd();
            static Directory getDirectory(const File &file);
            static bool isDirectory(const std::string &path);
        };

        //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        string Directory::getPath() const
        {
            return dirname;
        }
        
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        Directory::Directory(std::string dir_)
        {
            dirname = dir_;
        }

        //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/// @todo Incomplete algorithm
        Directory::Directory(const File &file)
        {
            string root, path = file.getPath();
            int lpos = path.size() - 1;
            while( lpos > 0 && path[lpos] == '/' || path[lpos] == '\\' ) --lpos;
            while(lpos > 0 && path[lpos] != '/' && path[lpos] != '\\')
                --lpos;
            for(int i=0; i<lpos; ++i)
                root = root + path[i];

            dirname = root;
        }

        //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        Directory Directory::child(const string &child_name) const
        {
#ifdef INFO_OS_WINDOWS
            return Directory(dirname + "\\" + child_name);
#else
            return Directory(dirname + "/" + child_name);
#endif
        }
        
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//        
        bool Directory::list(vector<Utils::Directory> &folders, vector<Utils::File> &files) const
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
/// @todo: 1- Do a BFS
/// @todo: 2- It's not optimized
/// append: do not clear files vector
/// pattern: a valid regular expression for search (see utils/regex.hpp for more information)
        bool Directory::findFiles(vector<File> &files, const string pattern, const bool append) const
        {
            Core::Logger::logVariable("[Directory] Searching ", dirname);
    
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
                    Core::Logger::logItLn("[Directory] findFiles invalid regex pattern");

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
                        Core::Logger::logVariable("[File]", buffer);

                        if( search && !rex.match(buffer) )
                        {
                            Core::Logger::logVariable("[File] Not Match", buffer);
                            break;
                        }
               

                        Core::Logger::logVariable("[File] Match", buffer);
                
                        files.push_back( File( string(buffer) ) );
                        break;

                    case DT_DIR:
                        /* Scan sub-directory recursively */
                        if (strcmp (ent->d_name, ".") != 0  
                            &&  strcmp (ent->d_name, "..") != 0)
                        {
                            // cout << "Going after " << ent->d_name << endl;
                            Directory::child(std::string(ent->d_name)).findFiles(files, pattern, true);
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
                Core::Logger::logVariable("[Directory] Cannot open directory ", dirname);
                return false;
            }

            return true;
        }

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        bool Directory::create()
        {
            bool res;
#if defined(INFO_OS_WINDOWS)
            res = ( CreateDirectory(dirname.c_str(), NULL) != 0 );
#else
            /// @TODO: 0750 is not a good idea
            res = ( mkdir(dirname.c_str(), 0750) == 0 );
            // return _mkdir(dir) == 0;
#endif
            Core::Logger::logVariable("Creating new dir, result", res);
            return res;
        }

        //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        Directory Directory::createSubDirectory(const string &name)
        {
            Directory child = Directory::child(name);
            child.create();
            return child;
        }
        
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        bool Directory::exists () const
        {
            DIR *dir;
            dir = opendir (dirname.c_str());

            if( dir )
            {
                closedir(dir);
                return true;
            }
            return false;
        }
        
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        bool Directory::isDirectory(const std::string &path)
        {
            return Directory(path).exists();
        }

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/// @todo Incomplete algorithm
        Directory Directory::parent() const
        {
            string root;
            int lpos = dirname.size() - 1;
            while( lpos > 0 && dirname[lpos] == '/' || dirname[lpos] == '\\' ) --lpos;
            while(lpos > 0 && dirname[lpos] != '/' && dirname[lpos] != '\\')
                --lpos;
            for(int i=0; i<lpos; ++i)
                root = root + dirname[i];
            return root;
        }

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        bool Directory::remove()
        {
#if defined(INFO_OS_WINDOWS)
            return RemoveDirectory((LPCSTR)folder.c_str()) != 0;
#else
            DIR*            dp;
            struct dirent*  ep;
            string          buf, tmp;

            dp = opendir(dirname.c_str());

            if( !dp ) return false;
            while ((ep = readdir(dp)) != NULL)
            {
                tmp = ep->d_name;
                if( tmp == "." || tmp == ".." )
                    continue;

                std::string rpath = dirname + "/" + tmp;
                if ( Directory::isDirectory(rpath) )
                {
                    Core::Logger::logVariable("[Directory] Removing directory ", rpath);
                    Directory(rpath).remove();
                }
                else
                {
                    if( !File(rpath).remove() )
                    {
                        Core::Logger::logVariable("[Directory] Unable to remove file ", rpath);
                        return false;
                    }
                }
            }

            closedir(dp);
        
            return rmdir(dirname.c_str()) == 0;
#endif
        }

        //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        Directory Directory::cwd()
        {
            char buf[256];
#ifdef INFO_OS_WINDOWS
            _getcwd(buf, 255);
#else
            getcwd(buf, 255);
#endif
            return Directory( buf );
        }

        //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/// @todo Incomplete algorithm
        Directory Directory::getDirectory(const File &file)
        {
            return Directory(file);
        }

    } // utility
} // grav

#endif // _GVN_DIRECTORY_HEAD_
