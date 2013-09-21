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
 * @brief Zip
 *
 */

#ifndef _GVN_ZIP_HEAD_
#define _GVN_ZIP_HEAD_

#include <graviton.hpp>
#include <string>
#include <external/zlib/zlib.h>
using namespace std;

namespace GraVitoN
{
    namespace Utils
    {
        class Zip
        {
            static int getMaxCompressBlock( int nLenSrc )
            {
                int n16kBlocks = (nLenSrc+16383) / 16384; // round up any fraction of a block
                return ( nLenSrc + 6 + (n16kBlocks*5) );
            }
        };
            
        class ZipFile : public Utils::Zip, public Utils::File
        {
        private:
            string zfile;
            
        public:
            ZipFile(std::string in_file);

            bool decompress(const string &out_file);
            bool compress(const string &out_file);
        };  

        template<class ZM_Type>
        class ZipMemory : public Utils::Zip, public Core::Memory<ZM_Type>
        {
            bool compress();
            bool decompress();
        };
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
            

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
            bool decompressFile(const string &in_file, const string &out_file, const unsigned int block_size = 256)
            {
                gzFile p_in_file = gzopen(in_file.c_str(), "rb");
                FILE *p_out_file = fopen(out_file.c_str(), "wb");
                if (!p_in_file || !p_out_file)
                    return false;

                char buffer[block_size];
                int num_read = 0;
                // size_t total = 0;
                while ((num_read = gzread(p_in_file, buffer, sizeof(buffer))) > 0)
                {
                    fwrite(buffer, 1, num_read, p_out_file);
                    // total += num_read;
                }

                gzclose(p_in_file);
                fclose(p_out_file);

                return true;
            }

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
            bool compressFile(const string &in_file, const string &out_file, const unsigned int block_size = 256)
            {
                FILE *p_in_file = fopen(in_file.c_str(), "rb");
                gzFile p_out_file = gzopen(out_file.c_str(), "wb");
                if (!p_in_file || !p_out_file)
                    return false;

                char buffer[block_size];
                int num_read = 0;
                // size_t total = 0;
                while ((num_read = fread(buffer, 1, sizeof(buffer), p_in_file)) > 0)
                {
                    // total +=
                    gzwrite(p_out_file, buffer, num_read);
                }

                gzclose(p_out_file);
                fclose(p_in_file);

                return true;
            }

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
            bool compressBuffer(const unsigned char* src, const size_t &src_len, unsigned char *&dst, size_t &dst_len)
            {
                z_stream zs;                        // z_stream is zlib's control structure
                memset(&zs, 0, sizeof(zs));

                if (deflateInit(&zs, Z_BEST_COMPRESSION) != Z_OK)
                    return false;

                zs.next_in = (Bytef*)src;
                zs.avail_in = src_len;           // set the z_stream's input

                int ret;
                unsigned char outbuffer[32768], *buf_ptr;
                dst_len = 0;
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

                dst = (unsigned char*)malloc(dst_len);
                memcpy((void*)dst, (void*)outbuffer, dst_len);

                return true;
            }


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
            bool decompressBuffer(const unsigned char* src, const size_t &src_len, unsigned char *&dst, size_t &dst_len)
            {
                z_stream zs;                        // z_stream is zlib's control structure
                memset(&zs, 0, sizeof(zs));

                if (inflateInit(&zs) != Z_OK)
                    return false;

                zs.next_in = (Bytef*)src;
                zs.avail_in = src_len;           // set the z_stream's input

                int ret;
                unsigned char outbuffer[32768], *buf_ptr;
                dst_len = 0;
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

                dst = (unsigned char*)malloc(dst_len);

                memcpy((void*)dst, (void*)outbuffer, dst_len);

                return true;
            }

        }
    }
}
#endif
