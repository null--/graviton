#ifndef _GVN_CRYPTO_ASYM_HEAD_
#define _GVN_CRYPTO_ASYM_HEAD_

#include <graviton.hpp>
#include <core/logger.hpp>
#include <string>
#include <external/openssl/include/openssl/rsa.h>
#include <external/openssl/include/openssl/x509v3.h>
#include <external/openssl/include/openssl/pem.h>
#include <external/openssl/include/openssl/pem2.h>
#include <external/openssl/include/openssl/pkcs7.h>
#include <external/openssl/include/openssl/pkcs12.h>
#include <external/openssl/include/openssl/evp.h>

using namespace std;

namespace GraVitoN
{
namespace Utils
{

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/// @todo generate ca and pub/priv key and pem file
namespace Crypto
{

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
EVP_PKEY *loadPublicKey(const string &certfile)
{
    FILE *fp = fopen (certfile.c_str(), "r");
    X509 *x509;
    EVP_PKEY *pkey;

    if (!fp)
        return _null_;

    x509 = PEM_read_X509(fp, _null_, 0, _null_);
    if (x509 == _null_)
    {
        // ERR_print_errors_fp (stderr);
        // Core::Logger::logItLn("Invalid x509");
        return _null_;
    }

    fclose (fp);
    pkey=X509_extract_key(x509);
    X509_free(x509);

    if (pkey == _null_)
    {
        // ERR_print_errors_fp (stderr);
        // Core::Logger::logItLn("Public key loading failed");
    }

    return pkey;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
EVP_PKEY *loadPrivateKey(const string &keyfile)
{
    FILE *fp = fopen(keyfile.c_str(), "r");
    EVP_PKEY *pkey;

    if (!fp)
        return _null_;

    pkey = PEM_read_PrivateKey(fp, _null_, 0, _null_);
    fclose (fp);

    // if (pkey == _null_)
        // ERR_print_errors_fp (stderr);

    return pkey;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
int rsaEncrypt_Public(const unsigned char *data, int data_size, const string &publickey_file, unsigned char *&buf)
{
    EVP_PKEY *pubkey;
    pubkey = loadPublicKey(publickey_file);

    if( !pubkey )
    {
        Core::Logger::logItLn("RSA ENC: Unable to load pkey files");
        return -1;
    }

    buf = (unsigned char*)malloc(EVP_PKEY_size(pubkey) + 1);
    int len = RSA_public_encrypt(data_size, (unsigned char*)data, (unsigned char*)buf, pubkey->pkey.rsa,RSA_PKCS1_PADDING);

    if(len != EVP_PKEY_size(pubkey) )
    {
        // out << "PL: " << EVP_PKEY_size(pubkey) << endl;
        Core::Logger::logVariable("RSA ENC: failed", len);
        return -1;
    }

    // cout << len << endl;
    // buf[len] = '\0';

    EVP_PKEY_free(pubkey);

    return len;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
int rsaDecrypt_Private(const unsigned char *data, int data_size, const string &privatekey_file, unsigned char *&buf)
{
    EVP_PKEY *privkey;
    privkey = loadPrivateKey(privatekey_file);

    if( !privkey )
    {
        Core::Logger::logItLn("RSA ENC: Unable to load pkey files");
        return -1;
    }

    buf = (unsigned char*)malloc(EVP_PKEY_size(privkey) + 1);
    int len = RSA_private_decrypt(data_size, (unsigned char*)data, (unsigned char*)buf, privkey->pkey.rsa,RSA_PKCS1_PADDING);

    EVP_PKEY_free(privkey);
    free(buf);

    return len;
}

}
}
}
#endif
