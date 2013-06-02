#ifndef _GVN_CRYPTO_SYM_HEAD_
#define _GVN_CRYPTO_SYM_HEAD_

#include <graviton.hpp>
#include <core/logger.hpp>
#include <string>
#include <external/openssl/include/openssl/des.h>
#include <external/openssl/include/openssl/aes.h>
#include <external/openssl/include/openssl/blowfish.h>
#include <external/openssl/include/openssl/rc4.h>
#include <external/openssl/include/openssl/rc2.h>
#include <external/openssl/include/openssl/evp.h>

using namespace std;

namespace GraVitoN
{

namespace Utils
{

namespace Crypto
{

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
int base64Decode(const unsigned char *data,
                 const int data_size,
                 unsigned char *&res)
{
    res = new unsigned char[data_size+24];
    int ebuflen;
    EVP_ENCODE_CTX ectx;

    if( data_size < 0 ) return -1;

    // unsigned char *cdata = (unsigned char*) malloc(data_size+3);
    // memcpy((void*)cdata, (void*)data, (size_t)data_size);
    // cdata[data_size] = 10;
    // cdata[data_size+1] = 0;

    EVP_DecodeInit(&ectx);
    EVP_DecodeUpdate(&ectx, (unsigned char*)res, &ebuflen, (unsigned char *)data, (int)data_size);
    EVP_DecodeFinal(&ectx, (unsigned char*)res, &ebuflen);

    // free(cdata);

    return ebuflen;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
int base64Encode(const unsigned char *data,
                 const int data_size,
                 unsigned char *&res)
{
    res = new unsigned char[data_size+24];
    int ebuflen;
    EVP_ENCODE_CTX ectx;

    EVP_EncodeInit(&ectx);
    EVP_EncodeUpdate(&ectx, (unsigned char*)res, &ebuflen, (unsigned char *)data, (int)data_size);
    EVP_EncodeFinal(&ectx, (unsigned char*)res, &ebuflen);

    for(int i=ebuflen-4; i<ebuflen; ++i)
    {
        // cout << i << ":" << (int)res[i] << endl;
        if(res[i] == '\n' || res[i] == '\r' )
        {
            res[i] = '\0';
            ebuflen = i;
            break;
        }
    }
    return ebuflen;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/// AES 128 CBC + SHA1 Salt
int aesEncrypt(const unsigned char *data,
               const int data_size,
               const unsigned char *key_data,
               const int key_size,
               const string &salt,
               unsigned char *&res)
{
    EVP_CIPHER_CTX e_ctx;

    int i, nrounds = 1;
    unsigned char key[16], iv[16];

    i = EVP_BytesToKey(
                EVP_aes_128_cbc(),
                EVP_sha1(),
                ((salt.empty()) ? _null_ : (unsigned char*)salt.c_str()),
                (unsigned char*)key_data,
                key_size,
                nrounds,
                key,
                iv);
    if (i != 16) {
        Core::Logger::logVariable("[AES ENC] Invalid Key Size", i);
        return -1;
    }

    EVP_CIPHER_CTX_init(&e_ctx);
    EVP_EncryptInit_ex(&e_ctx, EVP_aes_128_cbc(), NULL, key, iv);

    int c_len = data_size + AES_BLOCK_SIZE + 1, f_len = 0;
    res =  (unsigned char *)malloc(c_len);
    EVP_EncryptInit_ex(&e_ctx, NULL, NULL, NULL, NULL);
    // cout << data << data_size << endl;
    EVP_EncryptUpdate(&e_ctx, res, &c_len, (unsigned char*)data, data_size);
    EVP_EncryptFinal_ex(&e_ctx, res+c_len, &f_len);

    EVP_CIPHER_CTX_cleanup(&e_ctx);

    int len = c_len + f_len;

    res[len] = '\0';

    return len;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/// AES 128 CBC + SHA1 Salt
int aesDecrypt(const unsigned char *secret,
               const int secret_size,
               const unsigned char *key_data,
               const int key_size,
               const string &salt,
               unsigned char *&res)
{
    EVP_CIPHER_CTX d_ctx;

    int i, nrounds = 1;
    unsigned char key[16], iv[16];

    i = EVP_BytesToKey(
                EVP_aes_128_cbc(),
                EVP_sha1(),
                ((salt.empty()) ? _null_ : (unsigned char*)salt.c_str()),
                (unsigned char*)key_data,
                key_size,
                nrounds,
                key,
                iv);
    if (i != 16 ) {
        Core::Logger::logVariable("[AES DEC] Invalid Key Size", i);
        return -1;;
    }

    EVP_CIPHER_CTX_init(&d_ctx);
    EVP_DecryptInit_ex(&d_ctx, EVP_aes_128_cbc(), NULL, key, iv);

    int p_len = secret_size, f_len = 0;
    res =  (unsigned char*)malloc(p_len);
    EVP_DecryptInit_ex(&d_ctx, NULL, NULL, NULL, NULL);
    EVP_DecryptUpdate(&d_ctx, res, &p_len, (unsigned char*)secret, secret_size);
    EVP_DecryptFinal_ex(&d_ctx, res+p_len, &f_len);

    EVP_CIPHER_CTX_cleanup(&d_ctx);

    int len = p_len + f_len;
    res[len] = '\0';

    return len;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/// DES EDE3 CBC + SHA1 Salt
int des3Encrypt(const unsigned char *data, const int data_size, const unsigned char *key_data, const int key_size, const string &salt, unsigned char *&res)
{
    EVP_CIPHER_CTX e_ctx;

    int i, nrounds = 1;
    unsigned char key[16], iv[16];

    i = EVP_BytesToKey(
                EVP_des_ede3_cbc(),
                EVP_sha1(),
                ((salt.empty()) ? _null_ : (unsigned char*)salt.c_str()),
                (unsigned char*)key_data,
                key_size,
                nrounds,
                key,
                iv);
    if (i != 24) {
        Core::Logger::logVariable("[DES ENC] Invalid Key Size", i);
        return -1;;
    }

    EVP_CIPHER_CTX_init(&e_ctx);
    EVP_EncryptInit_ex(&e_ctx, EVP_des_ede3_cbc(), NULL, key, iv);

    int c_len = data_size + 8 + 1, f_len = 0;
    res =  (unsigned char *)malloc(c_len);
    EVP_EncryptInit_ex(&e_ctx, NULL, NULL, NULL, NULL);
    EVP_EncryptUpdate(&e_ctx, res, &c_len, (unsigned char*)data, data_size);
    EVP_EncryptFinal_ex(&e_ctx, res+c_len, &f_len);

    EVP_CIPHER_CTX_cleanup(&e_ctx);

    int len = c_len + f_len;

    res[len] = '\0';


    return len;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/// DES EDE3 CBC + SHA1 Salt
int des3Decrypt(const unsigned char *secret, const int secret_size, const unsigned char *key_data, const int key_size, const string &salt, unsigned char *&res)
{
    EVP_CIPHER_CTX d_ctx;

    int i, nrounds = 1;
    unsigned char key[16], iv[16];

    i = EVP_BytesToKey(
                EVP_des_ede3_cbc(),
                EVP_sha1(),
                ((salt.empty()) ? _null_ : (unsigned char*)salt.c_str()),
                (unsigned char*)key_data,
                key_size,
                nrounds,
                key,
                iv);
    if (i != 24) {
        Core::Logger::logVariable("[DES DEC] Invalid Key Size", i);
        return -1;;
    }

    EVP_CIPHER_CTX_init(&d_ctx);
    EVP_DecryptInit_ex(&d_ctx, EVP_des_ede3_cbc(), NULL, key, iv);

    int p_len = secret_size, f_len = 0;
    res =  (unsigned char*)malloc(p_len);
    EVP_DecryptInit_ex(&d_ctx, NULL, NULL, NULL, NULL);
    EVP_DecryptUpdate(&d_ctx, res, &p_len, (unsigned char*)secret, secret_size);
    EVP_DecryptFinal_ex(&d_ctx, res+p_len, &f_len);

    EVP_CIPHER_CTX_cleanup(&d_ctx);

    int len = p_len + f_len;
    res[len] = '\0';



    return len;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/// Blowfish CBC + SHA1 Salt
int blowfishEncrypt(const unsigned char *data, const int data_size, const unsigned char *key_data, const int key_size, const string &salt, unsigned char *&res)
{
    EVP_CIPHER_CTX e_ctx;

    int i, nrounds = 1;
    unsigned char key[16], iv[16];

    i = EVP_BytesToKey(
                EVP_bf_cbc(),
                EVP_sha1(),
                ((salt.empty()) ? _null_ : (unsigned char*)salt.c_str()),
                (unsigned char*)key_data,
                key_size,
                nrounds,
                key,
                iv);
    if (i != 16) {
        Core::Logger::logVariable("[BF ENC] Invalid Key Size", i);
        return -1;;
    }

    EVP_CIPHER_CTX_init(&e_ctx);
    EVP_EncryptInit_ex(&e_ctx, EVP_bf_cbc(), NULL, key, iv);

    int c_len = data_size + BF_BLOCK + 1, f_len = 0;
    res =  (unsigned char *)malloc(c_len);
    EVP_EncryptInit_ex(&e_ctx, NULL, NULL, NULL, NULL);
    EVP_EncryptUpdate(&e_ctx, res, &c_len, (unsigned char*)data, data_size);
    EVP_EncryptFinal_ex(&e_ctx, res+c_len, &f_len);

    EVP_CIPHER_CTX_cleanup(&e_ctx);

    int len = c_len + f_len;

    res[len] = '\0';



    return len;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/// Blowfish CBC + SHA1 Salt
int blowfishDecrypt(const unsigned char *secret, const int secret_size, const unsigned char *key_data, const int key_size, const string &salt, unsigned char *&res)
{
    EVP_CIPHER_CTX d_ctx;

    int i, nrounds = 1;
    unsigned char key[16], iv[16];

    i = EVP_BytesToKey(
                EVP_bf_cbc(),
                EVP_sha1(),
                ((salt.empty()) ? _null_ : (unsigned char*)salt.c_str()),
                (unsigned char*)key_data,
                key_size,
                nrounds,
                key,
                iv);
    if (i != 16) {
        Core::Logger::logVariable("[BF ENC] Invalid Key Size", i);
        return -1;;
    }

    EVP_CIPHER_CTX_init(&d_ctx);
    EVP_DecryptInit_ex(&d_ctx, EVP_bf_cbc(), NULL, key, iv);

    int p_len = secret_size, f_len = 0;
    res =  (unsigned char*)malloc(p_len);
    EVP_DecryptInit_ex(&d_ctx, NULL, NULL, NULL, NULL);
    EVP_DecryptUpdate(&d_ctx, res, &p_len, (unsigned char*)secret, secret_size);
    EVP_DecryptFinal_ex(&d_ctx, res+p_len, &f_len);

    EVP_CIPHER_CTX_cleanup(&d_ctx);

    int len = p_len + f_len;
    res[len] = '\0';



    return len;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/// RC2 CBC + SHA1 Salt
int rc2Encrypt(const unsigned char *data, const int data_size, const unsigned char *key_data, const int key_size, const string &salt, unsigned char *&res)
{
    EVP_CIPHER_CTX e_ctx;

    int i, nrounds = 1;
    unsigned char key[16], iv[16];

    i = EVP_BytesToKey(
                EVP_rc2_cbc(),
                EVP_sha1(),
                ((salt.empty()) ? _null_ : (unsigned char*)salt.c_str()),
                (unsigned char*)key_data,
                key_size,
                nrounds,
                key,
                iv);
    if (i != 16) {
        Core::Logger::logVariable("[BF ENC] Invalid Key Size", i);
        return -1;;
    }

    EVP_CIPHER_CTX_init(&e_ctx);
    EVP_EncryptInit_ex(&e_ctx, EVP_rc2_cbc(), NULL, key, iv);

    int c_len = data_size + RC2_BLOCK + 1, f_len = 0;
    res =  (unsigned char *)malloc(c_len);
    EVP_EncryptInit_ex(&e_ctx, NULL, NULL, NULL, NULL);
    EVP_EncryptUpdate(&e_ctx, res, &c_len, (unsigned char*)data, data_size);
    EVP_EncryptFinal_ex(&e_ctx, res+c_len, &f_len);

    EVP_CIPHER_CTX_cleanup(&e_ctx);

    int len = c_len + f_len;

    res[len] = '\0';



    return len;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/// RC2 CBC + SHA1 Salt
int rc2Decrypt(const unsigned char *secret, const int secret_size, const unsigned char *key_data, const int key_size, const string &salt, unsigned char *&res)
{
    EVP_CIPHER_CTX d_ctx;

    int i, nrounds = 1;
    unsigned char key[16], iv[16];

    i = EVP_BytesToKey(
                EVP_rc2_cbc(),
                EVP_sha1(),
                ((salt.empty()) ? _null_ : (unsigned char*)salt.c_str()),
                (unsigned char*)key_data,
                key_size,
                nrounds,
                key,
                iv);
    if (i != 16) {
        Core::Logger::logVariable("[BF ENC] Invalid Key Size", i);
        return -1;;
    }

    EVP_CIPHER_CTX_init(&d_ctx);
    EVP_DecryptInit_ex(&d_ctx, EVP_rc2_cbc(), NULL, key, iv);

    int p_len = secret_size, f_len = 0;
    res =  (unsigned char*)malloc(p_len);
    EVP_DecryptInit_ex(&d_ctx, NULL, NULL, NULL, NULL);
    EVP_DecryptUpdate(&d_ctx, res, &p_len, (unsigned char*)secret, secret_size);
    EVP_DecryptFinal_ex(&d_ctx, res+p_len, &f_len);

    EVP_CIPHER_CTX_cleanup(&d_ctx);

    int len = p_len + f_len;
    res[len] = '\0';



    return len;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/// RC4 HMAC MD5 + SHA1 Salt
int rc4Encrypt(const unsigned char *data, const int data_size, const unsigned char *key_data, const int key_size, const string &salt, unsigned char *&res)
{
    EVP_CIPHER_CTX e_ctx;

    int i, nrounds = 1;
    unsigned char key[16], iv[16];

    i = EVP_BytesToKey(
                EVP_rc4_hmac_md5(),
                EVP_sha1(),
                ((salt.empty()) ? _null_ : (unsigned char*)salt.c_str()),
                (unsigned char*)key_data,
                key_size,
                nrounds,
                key,
                iv);
    if (i != 16) {
        Core::Logger::logVariable("[BF ENC] Invalid Key Size", i);
        return -1;;
    }

    EVP_CIPHER_CTX_init(&e_ctx);
    EVP_EncryptInit_ex(&e_ctx, EVP_rc4_hmac_md5(), NULL, key, iv);

    int c_len = data_size + 1, f_len = 0;
    res =  (unsigned char *)malloc(c_len);
    EVP_EncryptInit_ex(&e_ctx, NULL, NULL, NULL, NULL);
    EVP_EncryptUpdate(&e_ctx, res, &c_len, (unsigned char*)data, data_size);
    EVP_EncryptFinal_ex(&e_ctx, res+c_len, &f_len);

    EVP_CIPHER_CTX_cleanup(&e_ctx);

    int len = c_len + f_len;

    res[len] = '\0';



    return len;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/// RC4 HMAC MD5 + SHA1 Salt
int rc4Decrypt(const unsigned char *secret, const int secret_size, const unsigned char *key_data, const int key_size, const string &salt, unsigned char *&res)
{
    EVP_CIPHER_CTX d_ctx;

    int i, nrounds = 1;
    unsigned char key[16], iv[16];

    i = EVP_BytesToKey(
                EVP_rc4_hmac_md5(),
                EVP_sha1(),
                ((salt.empty()) ? _null_ : (unsigned char*)salt.c_str()),
                (unsigned char*)key_data,
                key_size,
                nrounds,
                key,
                iv);
    if (i != 16) {
        Core::Logger::logVariable("[BF ENC] Invalid Key Size", i);
        return -1;;
    }

    EVP_CIPHER_CTX_init(&d_ctx);
    EVP_DecryptInit_ex(&d_ctx, EVP_rc4_hmac_md5(), NULL, key, iv);

    int p_len = secret_size, f_len = 0;
    res =  (unsigned char*)malloc(p_len);
    EVP_DecryptInit_ex(&d_ctx, NULL, NULL, NULL, NULL);
    EVP_DecryptUpdate(&d_ctx, res, &p_len, (unsigned char*)secret, secret_size);
    EVP_DecryptFinal_ex(&d_ctx, res+p_len, &f_len);

    EVP_CIPHER_CTX_cleanup(&d_ctx);

    int len = p_len + f_len;
    res[len] = '\0';



    return len;
}

}

}
}
#endif
