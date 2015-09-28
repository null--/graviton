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

#include <core/memory.hpp>
#include <core/logger.hpp>
// #include <utils/directory.hpp>

#include <utils/regex.hpp>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <algorithm>
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

#if defined(INFO_OS_WINDOWS)
	#define SLASH '\\'
#else
	#define SLASH '/'
#endif

namespace GraVitoN
{
    namespace Utils
    {
        class Directory;
        
        class File
        {
        protected:
            std::string path;
            
        public:
            File(std::string path_);
            
            size_t size() const;

            template<class T_File>
            Core::Memory<T_File> load() const;

            template<class T_File>
            bool save(const Core::Memory<T_File> &data, bool append = false);

            template<class T_File>
            bool saveAsCppArray(const Core::Memory<T_File> &buffer);
                
            bool exists() const;
            bool create();
            bool remove();

            std::string getPath() const;

            string name() const;
            string extension() const;
        };

        //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        File::File(std::string path_)
        {
            path = path_;
        }
        
        //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        std::string File::getPath() const
        {
            return path;
        }

        //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        bool File::create()
        {
            if( !File::exists() )
            {
                Core::Memory<char> buf(0);
                File::save(buf);
            }
        }
        
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        size_t File::size() const
        {
            FILE *file = fopen((const char*)path.c_str(), "rb");
            fseek (file, 0, SEEK_END);
            size_t _size = ftell(file);
            fclose (file);
            return _size;
        }

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/**
 * @brief Load a binary file into an unsigned char array
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
        Core::Memory<T_File> File::load() const
        {
            Core::Memory<T_File> buffer(Config::MAX_FILE_SIZE);
            
            FILE *file;
	
            Core::Logger::logIt("[File] Reading Binary File... ");
            file = fopen(path.c_str(), "rb");
            if( file )
            {
                while( !feof(file) )
                {
                    unsigned char ch = fgetc(file);
                    //printf("%.9d < %.2x\n", size, (*buffer)[size]);
                    if(!feof(file))
                    {
                        buffer.insert(&ch, sizeof(ch));
                    }
                }
                Core::Logger::logIt("[Size: "); Core::Logger::logIt( buffer.size() );
                Core::Logger::logItLn("] DONE");
		
                fclose(file);
            }
            else
            {
                buffer.free();
            }
            return buffer;
        }

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/**
 * @brief Save your buffer as a binary file
 *
 * @param [in] buffer
 * Pointer to your buffer
 *
 * @return
 * True if buffer was saved to file successfully
 */
        template<class T_File>
        bool File::save(const Core::Memory<T_File> &buffer, const bool append)
        {
            FILE *outf;
            outf = (append)?fopen(path.c_str(), "ab+"):fopen(path.c_str(), "wb+");

            if( !outf )
                return false;
	
            Core::Logger::logItLn("[File] Writing to binary file... ");
            for(register unsigned long i = 0; i < buffer.size(); ++i)
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
 * @param [in] buffer
 * Pointer to your buffer
 *
 * @return
 * True if buffer was saved to file successfully
 */
        template<class T_File>
        bool File::saveAsCppArray(const Core::Memory<T_File> &buffer)
        {
            FILE *outf;
            outf = fopen(path.c_str(), "w+");

            if( !outf )
                return false;

            int mod = 0;
            fprintf(outf, "const unsigned long buf_len = %lu;\n"
                    "unsigned char buf[] = \n",
                    buffer.size());
            Core::Logger::logIt("[File] Writing to cpp file... ");
            Core::Logger::logIt("Size: ");
            Core::Logger::logIt("Size: ");
            for(unsigned long i = 0; i < buffer.size(); ++i)
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
        bool File::exists() const
        {
            FILE *f = fopen(path.c_str(), "r");
            if(!f)
                return false;
            fclose(f);
            return true;
        }

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        bool File::remove()
        {
            Core::Logger::logVariable("[File] Removing", path);
            return std::remove(path.c_str()) == 0;
        }

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        string File::name() const
        {
            string nm = "";
            int lpos = path.size() - 1;
            if (lpos < 0) return "";
            while( lpos >= 0 && path[lpos] != SLASH ) nm = path[lpos--] + nm;
            return nm;
        }

        //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        string File::extension() const
        {
            string ext;
            int lpos = path.size() - 1;
            while( lpos > 0 && path[lpos] != '.' ) ext = path[lpos--] + ext;
            std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
            return (lpos < 1) ? "" : ext;
        }

    } // utility
} // grav

#endif // _GVN_FILE_HEAD_
