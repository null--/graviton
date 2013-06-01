#define GVN_ACTIVATE_LOGGER

#include <graviton.hpp>
#include <iostream>
#include <core/thread.hpp>
#include <utils/crypto_hash.hpp>
#include <utils/crypto_sym.hpp>
#include <utils/crypto_asym.hpp>
#include <utils/optparser.hpp>
#include <cstdlib>

using namespace std;
using namespace GraVitoN;

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
int main ( int argc , char **argv)
{
    if( argc != 2 )
    {
        cout << "Usage: " << argv[0] << " <text to encrypt>" << endl;
        return 0;
    }

    /// STRESS TEST
    /// Mem: 1,016 K
    // while( 1 )
    // { Core::Thread::sleep(50);

    string test = argv[1];

    cout<< "Text: " << test << endl;
    unsigned char
            md5_hex[ Utils::Crypto::MD5_LEN ],
            sha1_hex[ Utils::Crypto::SHA1_LEN ],
            sha224_hex[ Utils::Crypto::SHA224_LEN ],
            sha256_hex[ Utils::Crypto::SHA256_LEN ],
            sha384_hex[ Utils::Crypto::SHA384_LEN ],
            sha512_hex[ Utils::Crypto::SHA512_LEN ],
            ripemd_hex[ Utils::Crypto::RIPEMD160_LEN ];

    cout << "-------------------- Testing Hashing Algorithms --------------------" << endl;
    Utils::Crypto::md5Hex(test, md5_hex);
    cout << "MD5:\t" << Utils::OptParser::hexToStr(md5_hex, Utils::Crypto::MD5_LEN) << endl;
    Utils::Crypto::sha1Hex(test, sha1_hex);
    // return 0;
    cout << "SHA1:\t" << Utils::OptParser::hexToStr(sha1_hex, Utils::Crypto::SHA1_LEN) << endl;
    Utils::Crypto::sha224Hex(test, sha224_hex);
    cout << "SHA224:\t" << Utils::OptParser::hexToStr(sha224_hex, Utils::Crypto::SHA224_LEN) << endl;
    Utils::Crypto::sha256Hex(test, sha256_hex);
    cout << "SHA256:\t" << Utils::OptParser::hexToStr(sha256_hex, Utils::Crypto::SHA256_LEN) << endl;
    Utils::Crypto::sha384Hex(test, sha384_hex);
    cout << "SHA384:\t" << Utils::OptParser::hexToStr(sha384_hex, Utils::Crypto::SHA384_LEN) << endl;
    Utils::Crypto::sha512Hex(test, sha512_hex);
    cout << "SHA512:\t" << Utils::OptParser::hexToStr(sha512_hex, Utils::Crypto::SHA512_LEN) << endl;
    Utils::Crypto::ripemd160Hex(test, ripemd_hex);
    cout << "RIPEMD:\t" << Utils::OptParser::hexToStr(ripemd_hex, Utils::Crypto::RIPEMD160_LEN) << endl;

    cout << endl;
    cout << "-------------------- Testing Symmetrical Algorithms --------------------" << endl;
    string aes_key = "THE GODDAMN SECRET KEY";

    string cipher, plain;
    cipher = Utils::Crypto::aesEncrypt(test, aes_key);
    cout << "AES ENC:\t" << Utils::OptParser::hexToStr((unsigned char*)cipher.c_str(), cipher.size()) << endl;
    plain = Utils::Crypto::aesDecrypt(cipher, aes_key);
    cout << "AES DEC:\t" << plain << endl;// Utils::OptParser::hexToStr((unsigned char*)cipher.c_str(), cipher.size()) << endl;
    cout << ((plain == test)?("Passed!"):("FAILED!")) << endl << endl;

    cipher = Utils::Crypto::des3Encrypt(test, aes_key);
    cout << "DES3 ENC:\t" << Utils::OptParser::hexToStr((unsigned char*)cipher.c_str(), cipher.size()) << endl;
    plain = Utils::Crypto::des3Decrypt(cipher, aes_key);
    cout << "DES3 DEC:\t" << plain << endl;// Utils::OptParser::hexToStr((unsigned char*)cipher.c_str(), cipher.size()) << endl;
    cout << ((plain == test)?("Passed!"):("FAILED!")) << endl << endl;

    cipher = Utils::Crypto::blowfishEncrypt(test, aes_key);
    cout << "BLOWFISH ENC:\t" << Utils::OptParser::hexToStr((unsigned char*)cipher.c_str(), cipher.size()) << endl;
    plain = Utils::Crypto::blowfishDecrypt(cipher, aes_key);
    cout << "BLOWFISH DEC:\t" << plain << endl;// Utils::OptParser::hexToStr((unsigned char*)cipher.c_str(), cipher.size()) << endl;
    cout << ((plain == test)?("Passed!"):("FAILED!")) << endl << endl;

    cipher = Utils::Crypto::rc2Encrypt(test, aes_key);
    cout << "RC2 ENC:\t" << Utils::OptParser::hexToStr((unsigned char*)cipher.c_str(), cipher.size()) << endl;
    plain = Utils::Crypto::rc2Decrypt(cipher, aes_key);
    cout << "RC2 DEC:\t" << plain << endl;// Utils::OptParser::hexToStr((unsigned char*)cipher.c_str(), cipher.size()) << endl;
    cout << ((plain == test)?("Passed!"):("FAILED!")) << endl << endl;

    cipher = Utils::Crypto::rc4Encrypt(test, aes_key);
    cout << "RC4 ENC:\t" << Utils::OptParser::hexToStr((unsigned char*)cipher.c_str(), cipher.size()) << endl;
    plain = Utils::Crypto::rc4Decrypt(cipher, aes_key);
    cout << "RC4 DEC:\t" << plain << endl;// Utils::OptParser::hexToStr((unsigned char*)cipher.c_str(), cipher.size()) << endl;
    cout << ((plain == test)?("Passed!"):("FAILED!")) << endl << endl;

    cout << endl;
    cout << "-------------------- Testing Asymmetrical Algorithms --------------------" << endl;
    // }
    return 0;
}
