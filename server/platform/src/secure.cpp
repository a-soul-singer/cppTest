#include "secure.h"

#include <memory>
#include <string.h>

#include "openssl/sha.h"
#include "openssl/evp.h"
#include "openssl/aes.h"
#include "openssl/bio.h"
#include "openssl/pem.h"

using namespace std;

// 密钥
static const unsigned char g_secret_key[] = "0123456789abcdef";
static const unsigned char g_iv[] = { "ilovechina#$%^&*" };

std::string SecureUtil::Sha256Salt(const std::string &pwd)
{
    /*
    PBKDF2(Password-Based Key Derivation Function)是一个用来导出密钥的函数。
    用来生成加密的密码，增加破解的难度，可以用来进行密码或者口令的加密存储。
    主要是盐值+pwd，经过多轮HMAC算法的计算，产生的密文。
    int PKCS5_PBKDF2_HMAC(const char *pass, // 密码
                          int passlen, // 密码长度
                          const unsigned char *salt, // 盐值
                          int saltlen, // 盐值的长度
                          int iter, // 迭代次数
                          const EVP_MD *digest, // 摘要算法
                          int keylen, // 生成的目标长度
                          unsigned char *out); // 生成的结果
    */
    unsigned char salt[] = "xyz123";
    unsigned char key[SHA256_DIGEST_LENGTH] = { 0 };
    PKCS5_PBKDF2_HMAC(pwd.c_str(), pwd.size(), salt, sizeof(salt), 10000, EVP_sha256(), sizeof(key), key);
    char buffer[SHA256_DIGEST_LENGTH * 2 + 1] = { 0 };
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        sprintf(buffer + i * 2, "%02x", key[i]);
    }
    return buffer;
}

std::string SecureUtil::AesEncryptByCbc(const std::string &str)
{
    /*
    int AES_set_encrypt_key(const unsigned char *userKey, const int bits, AES_KEY *key);
        userKey：密钥，必须16、24或32Bytes
        bits：密钥长度，128、192或256bit
        key：密钥指针
    */
    AES_KEY aes;
    AES_set_encrypt_key(g_secret_key, 128, &aes);
    size_t len = str.size();
    if (len % AES_BLOCK_SIZE == 0) {
        len = len;
    } else {
        len = (len / AES_BLOCK_SIZE + 1) * AES_BLOCK_SIZE;
    }
    auto sp = shared_ptr<unsigned char[]>(new unsigned char[len], [](unsigned char* p) {delete[] p; });
    unsigned char iv[AES_BLOCK_SIZE] = { 0 };
    memcpy(iv, g_iv, sizeof(iv));
    /*
    void AES_cbc_encrypt(const unsigned char *in, unsigned char *out, size_t length, const AES_KEY *key, unsigned char *ivec, const int enc);
        in：明文指针
        out：密文指针
        length：明文实际长度（不足一个块16Bytes会自动补0），但是密文的长度应该是补0后长度
        key：密钥指针
        ivec：初始化向量，必须为16字节且可读写。加密和解密使用的iv必须一致
        enc：AES_ENCRYPT或AES_DECRYPT
    */
    AES_cbc_encrypt(reinterpret_cast<const unsigned char*>(str.c_str()), sp.get(), len, &aes, iv, AES_ENCRYPT);
    auto buffer = shared_ptr<char[]>(new char[len * 2 + 1], [](char* p) {delete[] p; });
    for (size_t i = 0; i < len; ++i) {
        sprintf(buffer.get() + i * 2, "%02x", sp.get()[i]);
    }
    return buffer.get();
}

std::string SecureUtil::AesDecryptByCbc(const std::string &str)
{
    AES_KEY aes;
    AES_set_decrypt_key(g_secret_key, 128, &aes);
    int len = str.size() / 2;
    auto buffer = shared_ptr<unsigned char[]>(new unsigned char[len], [](unsigned char* p) {delete[] p; });
    const char* str_ = str.c_str();
    int val = 0;
    for (int i = 0; i < len; ++i) {
        sscanf(str_ + 2 * i, "%2x", &val);
        buffer[i] = val;
    }
    unsigned char iv[AES_BLOCK_SIZE] = { 0 };
    memcpy(iv, g_iv, sizeof(iv));
    auto sp = shared_ptr<unsigned char[]>(new unsigned char[len], [](unsigned char* p) {delete[] p; });
    AES_cbc_encrypt(buffer.get(), sp.get(), len, &aes, iv, AES_DECRYPT);
    return reinterpret_cast<char*>(sp.get());
}
