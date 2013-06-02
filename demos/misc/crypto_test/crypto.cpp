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

    int len;

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
    Utils::Crypto::md5Hex((unsigned char*)test.c_str(), test.size(), md5_hex);
    cout << "MD5:\t" << Utils::OptParser::hexToStr(md5_hex, Utils::Crypto::MD5_LEN) << endl;
    Utils::Crypto::sha1Hex((unsigned char*)test.c_str(), test.size(), sha1_hex);
    cout << "SHA1:\t" << Utils::OptParser::hexToStr(sha1_hex, Utils::Crypto::SHA1_LEN) << endl;
    Utils::Crypto::sha224Hex((unsigned char*)test.c_str(), test.size(), sha224_hex);
    cout << "SHA224:\t" << Utils::OptParser::hexToStr(sha224_hex, Utils::Crypto::SHA224_LEN) << endl;
    Utils::Crypto::sha256Hex((unsigned char*)test.c_str(), test.size(), sha256_hex);
    cout << "SHA256:\t" << Utils::OptParser::hexToStr(sha256_hex, Utils::Crypto::SHA256_LEN) << endl;
    Utils::Crypto::sha384Hex((unsigned char*)test.c_str(), test.size(), sha384_hex);
    cout << "SHA384:\t" << Utils::OptParser::hexToStr(sha384_hex, Utils::Crypto::SHA384_LEN) << endl;
    Utils::Crypto::sha512Hex((unsigned char*)test.c_str(), test.size(), sha512_hex);
    cout << "SHA512:\t" << Utils::OptParser::hexToStr(sha512_hex, Utils::Crypto::SHA512_LEN) << endl;
    Utils::Crypto::ripemd160Hex((unsigned char*)test.c_str(), test.size(), ripemd_hex);
    cout << "RIPEMD:\t" << Utils::OptParser::hexToStr(ripemd_hex, Utils::Crypto::RIPEMD160_LEN) << endl;

    cout << endl;
    cout << "-------------------- Testing Symmetrical Algorithms --------------------" << endl;
    unsigned char *dec, *enc;
    len = Utils::Crypto::base64Encode((unsigned char*)test.c_str(), test.size(), enc);
    cout << "BASE64 ENC:\t" << enc << endl;
    len = Utils::Crypto::base64Decode(enc, len, dec);
    cout << "BASE64 DEC:\t" << dec << endl;
    cout << ((string((char*)dec) == test)?("Passed!"):("-=> FAILED! <=-")) << endl << endl;
    free(dec);
    free(enc);

    string aes_key = "THE GODDAMN SECRET KEY";

    unsigned char *secret, *plain;
    len = Utils::Crypto::aesEncrypt((unsigned char*)test.c_str(), test.size(), (unsigned char*)aes_key.c_str(), (int)aes_key.size(),"",secret);
    cout << "AES ENC:\t" << Utils::OptParser::hexToStr((unsigned char*)secret, len) << endl;
    len = Utils::Crypto::aesDecrypt(secret, len, (unsigned char*)aes_key.c_str(), (int)aes_key.size(), "", plain);
    cout << "AES DEC:\t" << plain << endl;// Utils::OptParser::hexToStr((unsigned char*)secret, len) << endl;
    cout << ((test == string((char*)plain))?("Passed!"):("-=> FAILED! <=-")) << endl << endl;
    free(secret);
    free(plain);

    len = Utils::Crypto::des3Encrypt((unsigned char*)test.c_str(), test.size(), (unsigned char*)aes_key.c_str(), (int)aes_key.size(),"",secret);
    cout << "DES3 ENC:\t" << Utils::OptParser::hexToStr((unsigned char*)secret, len) << endl;
    len = Utils::Crypto::des3Decrypt(secret, len, (unsigned char*)aes_key.c_str(), (int)aes_key.size(), "", plain);
    cout << "DES3 DEC:\t" << plain << endl;// Utils::OptParser::hexToStr((unsigned char*)secret, len) << endl;
    cout << ((test == string((char*)plain))?("Passed!"):("-=> FAILED! <=-")) << endl << endl;
    free(secret);
    free(plain);

    len = Utils::Crypto::blowfishEncrypt((unsigned char*)test.c_str(), test.size(), (unsigned char*)aes_key.c_str(), (int)aes_key.size(),"",secret);
    cout << "BLOWFISH ENC:\t" << Utils::OptParser::hexToStr((unsigned char*)secret, len) << endl;
    len = Utils::Crypto::blowfishDecrypt(secret, len, (unsigned char*)aes_key.c_str(), (int)aes_key.size(), "", plain);
    cout << "BLOWFISH DEC:\t" << plain << endl;// Utils::OptParser::hexToStr((unsigned char*)secret, len) << endl;
    cout << ((test == string((char*)plain))?("Passed!"):("-=> FAILED! <=-")) << endl << endl;
    free(secret);
    free(plain);

    len = Utils::Crypto::rc2Encrypt((unsigned char*)test.c_str(), test.size(), (unsigned char*)aes_key.c_str(), (int)aes_key.size(),"",secret);
    cout << "RC2 ENC:\t" << Utils::OptParser::hexToStr((unsigned char*)secret, len) << endl;
    len = Utils::Crypto::rc2Decrypt(secret, len, (unsigned char*)aes_key.c_str(), (int)aes_key.size(), "", plain);
    cout << "RC2 DEC:\t" << plain << endl;// Utils::OptParser::hexToStr((unsigned char*)secret, len) << endl;
    cout << ((test == string((char*)plain))?("Passed!"):("-=> FAILED! <=-")) << endl << endl;
    free(secret);
    free(plain);

    len = Utils::Crypto::rc4Encrypt((unsigned char*)test.c_str(), test.size(), (unsigned char*)aes_key.c_str(), (int)aes_key.size(),"",secret);
    cout << "RC4 ENC:\t" << Utils::OptParser::hexToStr((unsigned char*)secret, len) << endl;
    len = Utils::Crypto::rc4Decrypt(secret, len, (unsigned char*)aes_key.c_str(), (int)aes_key.size(), "", plain);
    cout << "RC4 DEC:\t" << plain << endl;// Utils::OptParser::hexToStr((unsigned char*)secret, len) << endl;
    cout << ((test == string((char*)plain))?("Passed!"):("-=> FAILED! <=-")) << endl << endl;
    free(secret);
    free(plain);

    cout << endl;
    cout << "-------------------- Testing Asymmetrical Algorithms --------------------" << endl;
    string pub_key = "cert.pem", priv_key = "privkey.pem";
    // string pub_key = "cert.pem", priv_key = "privkey.pem";
    len = Utils::Crypto::rsaEncrypt_Public((unsigned char*)test.c_str(), (int)test.size() + 1, pub_key, secret);
    cout << "RSA ENC:\t" << Utils::OptParser::hexToStr(secret, len) << endl;
    len = Utils::Crypto::rsaDecrypt_Private(secret, len, priv_key, plain);
    cout << "RSA DEC:\t" << plain << endl;
    cout << ((test == string((char*)plain))?("Passed!"):("-=> FAILED! <=-")) << endl << endl;

    return 0;
}
