#include "aesencryptionmanager.h"

AESEncryptionManager::AESEncryptionManager(QObject *parent): QObject(parent)
{

}

//加密
QString AESEncryptionManager::encrypt(const QString &plaintext, const QString &key, const QString &iv)
{
    QAESEncryption encryption(QAESEncryption::AES_128, QAESEncryption::CBC, QAESEncryption::ISO);
    QByteArray enBA = encryption.encode(plaintext.toUtf8(), key.toUtf8(), iv.toUtf8());
    QByteArray enBABase64 = enBA.toBase64();
    return QString(enBABase64);
}

//解密
QString AESEncryptionManager::decrypt(const QString &base64CipherText, const QString &key, const QString &iv)
{
    QByteArray enBA = base64Decode(base64CipherText);
    QAESEncryption encryption(QAESEncryption::AES_128, QAESEncryption::CBC, QAESEncryption::ISO);
    QByteArray deBA = encryption.decode(enBA, key.toUtf8(), iv.toUtf8());
    return QString(QAESEncryption::RemovePadding(deBA, QAESEncryption::ISO));
}

QByteArray AESEncryptionManager::base64Decode(const QString &base64String)
{
    return QByteArray::fromBase64(base64String.toUtf8());
}
