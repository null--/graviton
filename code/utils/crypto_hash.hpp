#ifndef _GVN_CRYPTO_HASH_HEAD_
#define _GVN_CRYPTO_HASH_HEAD_

#include <graviton.hpp>
#include <core/logger.hpp>
#include <string>
#include <limits>
#include <external/openssl/include/openssl/md5.h>
#include <external/openssl/include/openssl/sha.h>
#include <external/openssl/include/openssl/ripemd.h>

using namespace std;

namespace GraVitoN
{
namespace Utils
{

namespace Crypto
{

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
const size_t MD5_LEN = MD5_DIGEST_LENGTH;
void md5Hex(const string &data, unsigned char digest[MD5_LEN])
{
    MD5((const unsigned char*)data.c_str(), data.size(), (unsigned char*)digest);
    // MD5_CTX ctx;
    // MD5_Init(&ctx);
    // MD5_Update(&ctx, (unsigned char*)data.c_str(), data.size());
    // MD5_Final(digest, &ctx);
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
const size_t SHA1_LEN = SHA_DIGEST_LENGTH;
void sha1Hex(const string &data, unsigned char digest[SHA1_LEN])
{
    SHA1((const unsigned char*)data.c_str(), data.size(), (unsigned char*)digest);
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
const size_t SHA224_LEN = SHA224_DIGEST_LENGTH;
void sha224Hex(const string &data, unsigned char digest[SHA224_LEN])
{
    SHA224((const unsigned char*)data.c_str(), data.size(), (unsigned char*)digest);
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
const size_t SHA256_LEN = SHA256_DIGEST_LENGTH;
void sha256Hex(const string &data, unsigned char digest[SHA256_LEN])
{
    SHA256((const unsigned char*)data.c_str(), data.size(), (unsigned char*)digest);
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
const size_t SHA384_LEN = SHA384_DIGEST_LENGTH;
void sha384Hex(const string &data, unsigned char digest[SHA384_LEN])
{
    SHA384((const unsigned char*)data.c_str(), data.size(), (unsigned char*)digest);
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
const size_t SHA512_LEN = SHA512_DIGEST_LENGTH;
void sha512Hex(const string &data, unsigned char digest[SHA512_LEN])
{
    SHA512((const unsigned char*)data.c_str(), data.size(), (unsigned char*)digest);
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
const size_t RIPEMD160_LEN = RIPEMD160_DIGEST_LENGTH;
void ripemd160Hex(const string &data, unsigned char digest[RIPEMD160_LEN])
{
    RIPEMD160((const unsigned char*)data.c_str(), data.size(), (unsigned char*)digest);
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
/// Source: stackoverflow - Omnifarious
const char BASE64_TABLE[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
const char BASE64_TABLE_REV[128] = {
   64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
   64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
   64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 62, 64, 64, 64, 63,
   52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 64, 64, 64, 64, 64, 64,
   64,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
   15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 64, 64, 64, 64, 64,
   64, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
   41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 64, 64, 64, 64, 64
};

}

}
}
#endif
