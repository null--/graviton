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
void md5Hex(const unsigned char *data, const int data_size, unsigned char digest[MD5_LEN])
{
    MD5((const unsigned char*)data, data_size, (unsigned char*)digest);
    // MD5_CTX ctx;
    // MD5_Init(&ctx);
    // MD5_Update(&ctx, (unsigned char*)data, data_size);
    // MD5_Final(digest, &ctx);
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
const size_t SHA1_LEN = SHA_DIGEST_LENGTH;
void sha1Hex(const unsigned char *data, const int data_size, unsigned char digest[SHA1_LEN])
{
    SHA1((const unsigned char*)data, data_size, (unsigned char*)digest);
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
const size_t SHA224_LEN = SHA224_DIGEST_LENGTH;
void sha224Hex(const unsigned char *data, const int data_size, unsigned char digest[SHA224_LEN])
{
    SHA224((const unsigned char*)data, data_size, (unsigned char*)digest);
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
const size_t SHA256_LEN = SHA256_DIGEST_LENGTH;
void sha256Hex(const unsigned char *data, const int data_size, unsigned char digest[SHA256_LEN])
{
    SHA256((const unsigned char*)data, data_size, (unsigned char*)digest);
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
const size_t SHA384_LEN = SHA384_DIGEST_LENGTH;
void sha384Hex(const unsigned char *data, const int data_size, unsigned char digest[SHA384_LEN])
{
    SHA384((const unsigned char*)data, data_size, (unsigned char*)digest);
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
const size_t SHA512_LEN = SHA512_DIGEST_LENGTH;
void sha512Hex(const unsigned char *data, const int data_size, unsigned char digest[SHA512_LEN])
{
    SHA512((const unsigned char*)data, data_size, (unsigned char*)digest);
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
const size_t RIPEMD160_LEN = RIPEMD160_DIGEST_LENGTH;
void ripemd160Hex(const unsigned char *data, const int data_size, unsigned char digest[RIPEMD160_LEN])
{
    RIPEMD160((const unsigned char*)data, data_size, (unsigned char*)digest);
}

}

}
}
#endif
