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
/// Source: stackoverflow - Omnifarious
string base64Encode(const string &bindata)
{
   if (bindata.size() > (numeric_limits<string::size_type>::max() / 4u) * 3u) {
      Core::Logger::logItLn("Converting too large a string to base64.");
      return "";
   }

   const ::std::size_t binlen = bindata.size();
   /// Use = signs so the end is properly padded.
   string retval((((binlen + 2) / 3) * 4), '=');
   ::std::size_t outpos = 0;
   int bits_collected = 0;
   unsigned int accumulator = 0;
   const string::const_iterator binend = bindata.end();

   for (string::const_iterator i = bindata.begin(); i != binend; ++i)
   {
      accumulator = (accumulator << 8) | (*i & 0xffu);
      bits_collected += 8;
      while (bits_collected >= 6)
      {
         bits_collected -= 6;
         retval[outpos++] = BASE64_TABLE[(accumulator >> bits_collected) & 0x3fu];
      }
   }
   if (bits_collected > 0)
   { /// Any trailing bits that are missing.
      assert(bits_collected < 6);
      accumulator <<= 6 - bits_collected;
      retval[outpos++] = BASE64_TABLE[accumulator & 0x3fu];
   }
   assert(outpos >= (retval.size() - 2));
   assert(outpos <= retval.size());
   return retval;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/// Source: stackoverflow - Omnifarious
string base64Decode(const string &ascdata)
{
   string retval;
   const string::const_iterator last = ascdata.end();
   int bits_collected = 0;
   unsigned int accumulator = 0;

   for (string::const_iterator i = ascdata.begin(); i != last; ++i)
   {
      const int c = *i;
      if (::std::isspace(c) || c == '=')
      {
         /// Skip whitespace and padding. Be liberal in what you accept.
         continue;
      }
      if ((c > 127) || (c < 0) || (BASE64_TABLE_REV[c] > 63))
      {
         Core::Logger::logItLn("This contains characters not legal in a base64 encoded string.");
         return "";
      }
      accumulator = (accumulator << 6) | BASE64_TABLE_REV[c];
      bits_collected += 6;
      if (bits_collected >= 8)
      {
         bits_collected -= 8;
         retval += (char)((accumulator >> bits_collected) & 0xffu);
      }
   }
   return retval;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/// AES 128 CBC + SHA1 Salt
string aesEncrypt(const string &data, const string &key_data, const string &salt = "")
{
    EVP_CIPHER_CTX e_ctx;

    int i, nrounds = 1;
    unsigned char key[16], iv[16];

    i = EVP_BytesToKey(
                EVP_aes_128_cbc(),
                EVP_sha1(),
                (unsigned char*)salt.c_str(),
                (unsigned char*)key_data.c_str(),
                key_data.size(),
                nrounds,
                key,
                iv);
    if (i != 16) {
        Core::Logger::logVariable("[AES ENC] Invalid Key Size", i);
        return "";
    }

    EVP_CIPHER_CTX_init(&e_ctx);
    EVP_EncryptInit_ex(&e_ctx, EVP_aes_128_cbc(), NULL, key, iv);

    int c_len = data.size() + AES_BLOCK_SIZE + 1, f_len = 0;
    unsigned char *ciphertext = (unsigned char *)malloc(c_len);
    EVP_EncryptInit_ex(&e_ctx, NULL, NULL, NULL, NULL);
    EVP_EncryptUpdate(&e_ctx, ciphertext, &c_len, (unsigned char*)data.c_str(), data.size());
    EVP_EncryptFinal_ex(&e_ctx, ciphertext+c_len, &f_len);

    EVP_CIPHER_CTX_cleanup(&e_ctx);

    int len = c_len + f_len;

    ciphertext[len] = '\0';
    string res = ((char*)ciphertext);
    free(ciphertext);
    return res;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/// AES 128 CBC + SHA1 Salt
string aesDecrypt(const string &cipher, const string &key_data, const string &salt = "")
{
    EVP_CIPHER_CTX d_ctx;

    int i, nrounds = 1;
    unsigned char key[16], iv[16];

    i = EVP_BytesToKey(
                EVP_aes_128_cbc(),
                EVP_sha1(),
                (unsigned char*)salt.c_str(),
                (unsigned char*)key_data.c_str(),
                key_data.size(),
                nrounds,
                key,
                iv);
    if (i != 16 ) {
        Core::Logger::logVariable("[AES DEC] Invalid Key Size", i);
        return "";
    }

    EVP_CIPHER_CTX_init(&d_ctx);
    EVP_DecryptInit_ex(&d_ctx, EVP_aes_128_cbc(), NULL, key, iv);

    int p_len = cipher.size(), f_len = 0;
    unsigned char *plaintext = (unsigned char*)malloc(p_len);
    EVP_DecryptInit_ex(&d_ctx, NULL, NULL, NULL, NULL);
    EVP_DecryptUpdate(&d_ctx, plaintext, &p_len, (unsigned char*)cipher.c_str(), cipher.size());
    EVP_DecryptFinal_ex(&d_ctx, plaintext+p_len, &f_len);

    EVP_CIPHER_CTX_cleanup(&d_ctx);

    int len = p_len + f_len;
    plaintext[len] = '\0';
    string res = ((char*)plaintext);
    free(plaintext);
    return res;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/// DES EDE3 CBC + SHA1 Salt
string des3Encrypt(const string &data, const string &key_data, const string &salt = "")
{
    EVP_CIPHER_CTX e_ctx;

    int i, nrounds = 1;
    unsigned char key[16], iv[16];

    i = EVP_BytesToKey(
                EVP_des_ede3_cbc(),
                EVP_sha1(),
                (unsigned char*)salt.c_str(),
                (unsigned char*)key_data.c_str(),
                key_data.size(),
                nrounds,
                key,
                iv);
    if (i != 24) {
        Core::Logger::logVariable("[DES ENC] Invalid Key Size", i);
        return "";
    }

    EVP_CIPHER_CTX_init(&e_ctx);
    EVP_EncryptInit_ex(&e_ctx, EVP_des_ede3_cbc(), NULL, key, iv);

    int c_len = data.size() + 8 + 1, f_len = 0;
    unsigned char *ciphertext = (unsigned char *)malloc(c_len);
    EVP_EncryptInit_ex(&e_ctx, NULL, NULL, NULL, NULL);
    EVP_EncryptUpdate(&e_ctx, ciphertext, &c_len, (unsigned char*)data.c_str(), data.size());
    EVP_EncryptFinal_ex(&e_ctx, ciphertext+c_len, &f_len);

    EVP_CIPHER_CTX_cleanup(&e_ctx);

    int len = c_len + f_len;

    ciphertext[len] = '\0';
    string res = ((char*)ciphertext);
    free(ciphertext);
    return res;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/// DES EDE3 CBC + SHA1 Salt
string des3Decrypt(const string &cipher, const string &key_data, const string &salt = "")
{
    EVP_CIPHER_CTX d_ctx;

    int i, nrounds = 1;
    unsigned char key[16], iv[16];

    i = EVP_BytesToKey(
                EVP_des_ede3_cbc(),
                EVP_sha1(),
                (unsigned char*)salt.c_str(),
                (unsigned char*)key_data.c_str(),
                key_data.size(),
                nrounds,
                key,
                iv);
    if (i != 24) {
        Core::Logger::logVariable("[DES DEC] Invalid Key Size", i);
        return "";
    }

    EVP_CIPHER_CTX_init(&d_ctx);
    EVP_DecryptInit_ex(&d_ctx, EVP_des_ede3_cbc(), NULL, key, iv);

    int p_len = cipher.size(), f_len = 0;
    unsigned char *plaintext = (unsigned char*)malloc(p_len);
    EVP_DecryptInit_ex(&d_ctx, NULL, NULL, NULL, NULL);
    EVP_DecryptUpdate(&d_ctx, plaintext, &p_len, (unsigned char*)cipher.c_str(), cipher.size());
    EVP_DecryptFinal_ex(&d_ctx, plaintext+p_len, &f_len);

    EVP_CIPHER_CTX_cleanup(&d_ctx);

    int len = p_len + f_len;
    plaintext[len] = '\0';

    string res = ((char*)plaintext);
    free(plaintext);
    return res;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/// Blowfish CBC + SHA1 Salt
string blowfishEncrypt(const string &data, const string &key_data, const string &salt = "")
{
    EVP_CIPHER_CTX e_ctx;

    int i, nrounds = 1;
    unsigned char key[16], iv[16];

    i = EVP_BytesToKey(
                EVP_bf_cbc(),
                EVP_sha1(),
                (unsigned char*)salt.c_str(),
                (unsigned char*)key_data.c_str(),
                key_data.size(),
                nrounds,
                key,
                iv);
    if (i != 16) {
        Core::Logger::logVariable("[BF ENC] Invalid Key Size", i);
        return "";
    }

    EVP_CIPHER_CTX_init(&e_ctx);
    EVP_EncryptInit_ex(&e_ctx, EVP_bf_cbc(), NULL, key, iv);

    int c_len = data.size() + BF_BLOCK + 1, f_len = 0;
    unsigned char *ciphertext = (unsigned char *)malloc(c_len);
    EVP_EncryptInit_ex(&e_ctx, NULL, NULL, NULL, NULL);
    EVP_EncryptUpdate(&e_ctx, ciphertext, &c_len, (unsigned char*)data.c_str(), data.size());
    EVP_EncryptFinal_ex(&e_ctx, ciphertext+c_len, &f_len);

    EVP_CIPHER_CTX_cleanup(&e_ctx);

    int len = c_len + f_len;

    ciphertext[len] = '\0';

    string res = ((char*)ciphertext);
    free(ciphertext);
    return res;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/// Blowfish CBC + SHA1 Salt
string blowfishDecrypt(const string &cipher, const string &key_data, const string &salt = "")
{
    EVP_CIPHER_CTX d_ctx;

    int i, nrounds = 1;
    unsigned char key[16], iv[16];

    i = EVP_BytesToKey(
                EVP_bf_cbc(),
                EVP_sha1(),
                (unsigned char*)salt.c_str(),
                (unsigned char*)key_data.c_str(),
                key_data.size(),
                nrounds,
                key,
                iv);
    if (i != 16) {
        Core::Logger::logVariable("[BF ENC] Invalid Key Size", i);
        return "";
    }

    EVP_CIPHER_CTX_init(&d_ctx);
    EVP_DecryptInit_ex(&d_ctx, EVP_bf_cbc(), NULL, key, iv);

    int p_len = cipher.size(), f_len = 0;
    unsigned char *plaintext = (unsigned char*)malloc(p_len);
    EVP_DecryptInit_ex(&d_ctx, NULL, NULL, NULL, NULL);
    EVP_DecryptUpdate(&d_ctx, plaintext, &p_len, (unsigned char*)cipher.c_str(), cipher.size());
    EVP_DecryptFinal_ex(&d_ctx, plaintext+p_len, &f_len);

    EVP_CIPHER_CTX_cleanup(&d_ctx);

    int len = p_len + f_len;
    plaintext[len] = '\0';

    string res = ((char*)plaintext);
    free(plaintext);
    return res;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/// RC2 CBC + SHA1 Salt
string rc2Encrypt(const string &data, const string &key_data, const string &salt = "")
{
    EVP_CIPHER_CTX e_ctx;

    int i, nrounds = 1;
    unsigned char key[16], iv[16];

    i = EVP_BytesToKey(
                EVP_rc2_cbc(),
                EVP_sha1(),
                (unsigned char*)salt.c_str(),
                (unsigned char*)key_data.c_str(),
                key_data.size(),
                nrounds,
                key,
                iv);
    if (i != 16) {
        Core::Logger::logVariable("[BF ENC] Invalid Key Size", i);
        return "";
    }

    EVP_CIPHER_CTX_init(&e_ctx);
    EVP_EncryptInit_ex(&e_ctx, EVP_rc2_cbc(), NULL, key, iv);

    int c_len = data.size() + RC2_BLOCK + 1, f_len = 0;
    unsigned char *ciphertext = (unsigned char *)malloc(c_len);
    EVP_EncryptInit_ex(&e_ctx, NULL, NULL, NULL, NULL);
    EVP_EncryptUpdate(&e_ctx, ciphertext, &c_len, (unsigned char*)data.c_str(), data.size());
    EVP_EncryptFinal_ex(&e_ctx, ciphertext+c_len, &f_len);

    EVP_CIPHER_CTX_cleanup(&e_ctx);

    int len = c_len + f_len;

    ciphertext[len] = '\0';

    string res = ((char*)ciphertext);
    free(ciphertext);
    return res;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/// RC2 CBC + SHA1 Salt
string rc2Decrypt(const string &cipher, const string &key_data, const string &salt = "")
{
    EVP_CIPHER_CTX d_ctx;

    int i, nrounds = 1;
    unsigned char key[16], iv[16];

    i = EVP_BytesToKey(
                EVP_rc2_cbc(),
                EVP_sha1(),
                (unsigned char*)salt.c_str(),
                (unsigned char*)key_data.c_str(),
                key_data.size(),
                nrounds,
                key,
                iv);
    if (i != 16) {
        Core::Logger::logVariable("[BF ENC] Invalid Key Size", i);
        return "";
    }

    EVP_CIPHER_CTX_init(&d_ctx);
    EVP_DecryptInit_ex(&d_ctx, EVP_rc2_cbc(), NULL, key, iv);

    int p_len = cipher.size(), f_len = 0;
    unsigned char *plaintext = (unsigned char*)malloc(p_len);
    EVP_DecryptInit_ex(&d_ctx, NULL, NULL, NULL, NULL);
    EVP_DecryptUpdate(&d_ctx, plaintext, &p_len, (unsigned char*)cipher.c_str(), cipher.size());
    EVP_DecryptFinal_ex(&d_ctx, plaintext+p_len, &f_len);

    EVP_CIPHER_CTX_cleanup(&d_ctx);

    int len = p_len + f_len;
    plaintext[len] = '\0';

    string res = ((char*)plaintext);
    free(plaintext);
    return res;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/// RC4 HMAC MD5 + SHA1 Salt
string rc4Encrypt(const string &data, const string &key_data, const string &salt = "")
{
    EVP_CIPHER_CTX e_ctx;

    int i, nrounds = 1;
    unsigned char key[16], iv[16];

    i = EVP_BytesToKey(
                EVP_rc4_hmac_md5(),
                EVP_sha1(),
                (unsigned char*)salt.c_str(),
                (unsigned char*)key_data.c_str(),
                key_data.size(),
                nrounds,
                key,
                iv);
    if (i != 16) {
        Core::Logger::logVariable("[BF ENC] Invalid Key Size", i);
        return "";
    }

    EVP_CIPHER_CTX_init(&e_ctx);
    EVP_EncryptInit_ex(&e_ctx, EVP_rc4_hmac_md5(), NULL, key, iv);

    int c_len = data.size() + 1, f_len = 0;
    unsigned char *ciphertext = (unsigned char *)malloc(c_len);
    EVP_EncryptInit_ex(&e_ctx, NULL, NULL, NULL, NULL);
    EVP_EncryptUpdate(&e_ctx, ciphertext, &c_len, (unsigned char*)data.c_str(), data.size());
    EVP_EncryptFinal_ex(&e_ctx, ciphertext+c_len, &f_len);

    EVP_CIPHER_CTX_cleanup(&e_ctx);

    int len = c_len + f_len;

    ciphertext[len] = '\0';

    string res = ((char*)ciphertext);
    free(ciphertext);
    return res;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/// RC4 HMAC MD5 + SHA1 Salt
string rc4Decrypt(const string &cipher, const string &key_data, const string &salt = "")
{
    EVP_CIPHER_CTX d_ctx;

    int i, nrounds = 1;
    unsigned char key[16], iv[16];

    i = EVP_BytesToKey(
                EVP_rc4_hmac_md5(),
                EVP_sha1(),
                (unsigned char*)salt.c_str(),
                (unsigned char*)key_data.c_str(),
                key_data.size(),
                nrounds,
                key,
                iv);
    if (i != 16) {
        Core::Logger::logVariable("[BF ENC] Invalid Key Size", i);
        return "";
    }

    EVP_CIPHER_CTX_init(&d_ctx);
    EVP_DecryptInit_ex(&d_ctx, EVP_rc4_hmac_md5(), NULL, key, iv);

    int p_len = cipher.size(), f_len = 0;
    unsigned char *plaintext = (unsigned char*)malloc(p_len);
    EVP_DecryptInit_ex(&d_ctx, NULL, NULL, NULL, NULL);
    EVP_DecryptUpdate(&d_ctx, plaintext, &p_len, (unsigned char*)cipher.c_str(), cipher.size());
    EVP_DecryptFinal_ex(&d_ctx, plaintext+p_len, &f_len);

    EVP_CIPHER_CTX_cleanup(&d_ctx);

    int len = p_len + f_len;
    plaintext[len] = '\0';

    string res = ((char*)plaintext);
    free(plaintext);
    return res;
}

}

}
}
#endif
