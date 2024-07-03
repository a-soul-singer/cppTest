#ifndef SECURE_H
#define SECURE_H

#include <string>

class SecureUtil
{
public:
    /**
     * 使用盐值加密密码
     * \param pwd：密码
     * \return
     */
    static std::string Sha256Salt(const std::string& pwd);

    /**
     * AES CBC模式加密
     * \param str: 需要加密的明文
     * \return
     */
    static std::string AesEncryptByCbc(const std::string& str);
    
    /**
     * AES CBC模式解密
     * \param str：需要解密的密文
     * \return
     */
    static std::string AesDecryptByCbc(const std::string& str);
};

#endif