#ifndef AESENCRYPTIONMANAGER_H
#define AESENCRYPTIONMANAGER_H

#include <QObject>
#include <QByteArray>
#include <QDebug>
#include "AES\qaesencryption.h"

class AESEncryptionManager : public QObject
{
    Q_OBJECT
public:
    explicit AESEncryptionManager(QObject *parent = nullptr);

    QString encrypt(const QString &plaintext, const QString &key, const QString &iv);
    QString decrypt(const QString &base64CipherText, const QString &key, const QString &iv);

private:
    QByteArray base64Decode(const QString &base64String);
};

#endif // AESENCRYPTIONMANAGER_H
