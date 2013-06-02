#define GVN_ACTIVATE_LOGGER

#include <graviton.hpp>
#include <iostream>
#include <utils/files.hpp>
#include <utils/zip.hpp>
#include <utils/optparser.hpp>

using namespace std;

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
int main ( int argc , char **argv)
{
    if(argc != 2)
    {
        cout << "Usage: " << argv[0] << " <file name>" << endl;
        return 0;
    }

    string file_name = argv[1];
    size_t b_sz = 256; /// don't forget '\0'
    unsigned char buff[b_sz];
    for(size_t i=0; i<b_sz-1; ++i)
        buff[i] =  (i % ('z' - 'a' + 1)) + 'a';
    buff[b_sz-1] = '\0';

    size_t f_size, iz_size, oz_size, ib_size, ob_size;

    f_size = GraVitoN::Utils::File::getSize( file_name );
    cout << "Original File Size: " << f_size << " Bytes" << endl;
    GraVitoN::Utils::Zip::compressFile(file_name, file_name + ".gz");
    oz_size = GraVitoN::Utils::File::getSize( file_name + ".gz");
    cout << "Compressed File Size: " << oz_size << " Bytes" << endl;
    GraVitoN::Utils::Zip::decompressFile(file_name + ".gz", file_name + ".gz.decom");
    iz_size = GraVitoN::Utils::File::getSize( file_name + ".gz.decom");
    cout << "Decompressed File Size: " << iz_size << " Bytes" << endl;

    cout << endl;
    // cout << "Original buff: '" << buff << "'" << endl;
    unsigned char *izbuff = _null_, *ozbuff = _null_;
    GraVitoN::Utils::Zip::compressBuffer(buff, b_sz, izbuff, ib_size);
    cout << "Compressed buffer size: " << ib_size << " Bytes" << endl;
    // cout << "Compressed Buffer: '" << GraVitoN::Utils::OptParser::hexToStr(izbuff, ib_size) << "'" << endl;
    GraVitoN::Utils::Zip::decompressBuffer(izbuff, ib_size, ozbuff, ob_size);
    cout << "Decompressed buffer size: " << ob_size << " Bytes" << endl;
    // cout << "Decompressed Buffer: '" << ozbuff << "'" << endl;
    cout << "Result: " << ((string((char*)buff) == string((char*)ozbuff))?("Matched"):("UNMATCHED")) << endl;
    return 0;
}
