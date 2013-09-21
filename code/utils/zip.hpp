/**
 * @file
 *
 * @author  Sina Hatef Matbue ( _null_ ) <sinahatef.cpp@gmail.com>
 *
 * @section License
 *
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
 * @brief Zip ops
 *
 */

#ifndef _GVN_ZIP_HEAD_
#define _GVN_ZIP_HEAD_

#include <graviton.hpp>
#include <core/memory.hpp>
#include <string>
#include <external/zlib/zlib.h>
using namespace std;

namespace GraVitoN
{
    namespace Utils
    {
        //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        class Zip
        {
            public:
            static int getMaxCompressBlock( int nLenSrc = 256)
                {
                    int n16kBlocks = (nLenSrc+16383) / 16384; // round up any fraction of a block
                    return ( nLenSrc + 6 + (n16kBlocks*5) );
                }
        };

        //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        class ZipFile : public Utils::File
        {
        private:
            string zfile;
            
        public:
            ZipFile(const std::string &in_file) : File(in_file) {}

            ZipFile decompress(const string &out_file);
            ZipFile compress(const string &out_file);
        };  

        //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        template<class ZM_Type>
        class ZipMemory : public Core::Memory<ZM_Type>
        {
        public:
            ZipMemory(const gsize &size_ = 0) : Core::Memory<ZM_Type>(size_) {}

            Core::Memory<ZM_Type> compress();
            Core::Memory<ZM_Type> decompress();

            /// Memory = Memory operator
            ZipMemory<ZM_Type> & operator = (const Core::Memory<ZM_Type> &a)
                {
                    this->copy(a.address(), a.size());
                    return *this;
                }
        };

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        ZipFile ZipFile::decompress(const string &out_file)
        {
            gzFile p_in_file = gzopen(path.c_str(), "rb");
            FILE *p_out_file = fopen(out_file.c_str(), "wb");
            if (p_in_file && p_out_file)
            {
                char buffer[ Zip::getMaxCompressBlock() ];
                int num_read = 0;
                // size_t total = 0;
                while ((num_read = gzread(p_in_file, buffer, sizeof(buffer))) > 0)
                {
                    fwrite(buffer, 1, num_read, p_out_file);
                    // total += num_read;
                }

                gzclose(p_in_file);
                fclose(p_out_file);
            }
            return ZipFile(out_file);
        }

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        ZipFile ZipFile::compress(const string &out_file)
        {
            FILE *p_in_file = fopen(path.c_str(), "rb");
            gzFile p_out_file = gzopen(out_file.c_str(), "wb");
            if (p_in_file && p_out_file)
            {
                char buffer[Utils::Zip::getMaxCompressBlock()];
                int num_read = 0;
                // size_t total = 0;
                while ((num_read = fread(buffer, 1, sizeof(buffer), p_in_file)) > 0)
                {
                    // total +=
                    gzwrite(p_out_file, buffer, num_read);
                }

                gzclose(p_out_file);
                fclose(p_in_file);
            }
            return ZipFile(out_file);
        }

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        template<class ZM_Type>
        Core::Memory<ZM_Type> ZipMemory<ZM_Type>::compress()
        {
            z_stream zs;                        // z_stream is zlib's control structure
            memset(&zs, 0, sizeof(zs));

            if (deflateInit(&zs, Z_BEST_COMPRESSION) != Z_OK)
                return false;

            zs.next_in = (Bytef*)ZipMemory<ZM_Type>::address();
            zs.avail_in = ZipMemory<ZM_Type>::size();           // set the z_stream's input

            int ret;
            unsigned char outbuffer[32768], *buf_ptr;
            unsigned int dst_len = 0;
            // retrieve the compressed bytes blockwise
            buf_ptr = outbuffer;
            do
            {
                buf_ptr = (outbuffer + zs.total_out);
                zs.next_out = reinterpret_cast<Bytef*>(buf_ptr);
                zs.avail_out = sizeof(outbuffer) - zs.total_out - 1;

                ret = deflate(&zs, Z_FINISH);
            }
            while (ret == Z_OK);
            deflateEnd(&zs);

            dst_len = zs.total_out;
            if (ret != Z_STREAM_END)
            {
                return false;
            }

            Core::Memory<ZM_Type> dst(dst_len);
            dst.copy((ZM_Type*)outbuffer, dst_len);

            return dst;
        }


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
        template<class ZM_Type>
        Core::Memory<ZM_Type> ZipMemory<ZM_Type>::decompress()
        {
            z_stream zs;                        // z_stream is zlib's control structure
            memset(&zs, 0, sizeof(zs));

            if (inflateInit(&zs) != Z_OK)
                return false;

            zs.next_in = (Bytef*)ZipMemory<ZM_Type>::address();
            zs.avail_in = ZipMemory<ZM_Type>::size();           // set the z_stream's input

            int ret;
            unsigned char outbuffer[32768], *buf_ptr;
            unsigned int dst_len = 0;
            // retrieve the compressed bytes blockwise
            buf_ptr = outbuffer;
            do
            {
                buf_ptr = (outbuffer + zs.total_out);
                zs.next_out = reinterpret_cast<Bytef*>(buf_ptr);
                zs.avail_out = sizeof(outbuffer) - zs.total_out - 1;

                ret = inflate(&zs, 0);
            }
            while (ret == Z_OK);
            inflateEnd(&zs);

            dst_len = zs.total_out;
            if (ret != Z_STREAM_END)
            {
                return false;
            }

            Core::Memory<ZM_Type> dst(dst_len);
            dst.copy((ZM_Type*)outbuffer, dst_len);

            return dst;
        }

    } // utils
} // grav
#endif
