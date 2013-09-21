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
 * @brief test zip lib
 *
*/


#define GVN_ACTIVATE_LOGGER

#include <graviton.hpp>
#include <iostream>
#include <utils/file.hpp>
#include <utils/zip.hpp>

using namespace std;
using namespace GraVitoN::Utils;
using namespace GraVitoN::Core;

/// COMPILE: g++ -I ../../../../code -o zip_test zip_test.cpp -L ../../../../lib/zlib/linux-amd64/ -lzlib

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
int main ( int argc , char **argv)
{
    if(argc != 2)
    {
        cout << "Usage: " << argv[0] << " <file name>" << endl;
        return 0;
    }

    ZipFile zfile(argv[1]);
    
    /// Zip a file
    cout << "Original File Size: " << zfile.size() << " Bytes" << endl;

    /// Compress file
    ZipFile czf = zfile.compress(zfile.getPath() + ".gz");
    cout << "Compressed File Size: " << czf.size() << " Bytes" << endl;

    /// Decompress file
    ZipFile dzf = czf.decompress(czf.getPath() + ".decom");
    cout << "Decompressed File Size: " << dzf.size() << " Bytes" << endl;
    cout << "Ratio: " << (1.f - (float)czf.size() / (float)dzf.size()) * 100.f << "%" << endl;
    
    /// Zip a buffer
    ZipMemory<GraVitoN::guchar> buff(256);
    buff.zero();
    
    for(size_t i=0; i<buff.size(); ++i)
        buff[i] =  (i % ('z' - 'a' + 1)) + 'a';

    cout << endl;
    cout << "Original buff: '" << buff << "'" << endl;

    /// Compress buffer
    ZipMemory<unsigned char> izbuff, ozbuff;
    ozbuff = buff.compress();
    cout << "Compressed buffer size: " << ozbuff.size() << " Bytes" << endl;

    /// Decompress buffer
    // cout << "Compressed Buffer: '" << GraVitoN::Utils::OptParser::hexToStr(izbuff, ib_size) << "'" << endl;
    izbuff = ozbuff.decompress();
    cout << "Decompressed Buffer Size: " << izbuff.size() << " Bytes" << endl;
    cout << "Decompressed Buffer: '" << izbuff << "'" << endl;
    cout << "Ratio: " << (1.f - (float)ozbuff.size() / (float)izbuff.size()) * 100.f << "%" << endl;
    
    cout << "Result: " << ( (buff.toString() == izbuff.toString()) ? ("Matched") : ("UNMATCHED") ) << endl;
    return 0;
}
