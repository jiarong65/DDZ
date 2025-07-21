#ifndef AESCRYPTO_H
#define AESCRYPTO_H

#include <QObject>
#include<QByteArray>
#include <openssl/evp.h>
#include<QMap>
#include <openssl/aes.h>
#include <QCryptographicHash>


class AesCrypto:public QObject
{
    Q_OBJECT
public:
    using algorithmFunc=const EVP_CIPHER* (*)();
    enum Algorithm{
        //16BYTE
        AES_ECB_128,
        AES_CBC_128,
        AES_CFB_128,
        AES_OFB_128,
        AES_CTR_128,

        //24BYTE
        AES_ECB_192,
        AES_CBC_192,
        AES_CFB_192,
        AES_OFB_192,
        AES_CTR_192,

        //32BYTE
        AES_ECB_256,
        AES_CBC_256,
        AES_CFB_256,
        AES_OFB_256,
        AES_CTR_256,
    };
    //解密为0，加密为1
    enum CryptoType{
        DECRYPTO,ENCRYPTO
    };

    AesCrypto(Algorithm algorithm,QByteArray key,QObject* parent=nullptr);

    //加解密
    QByteArray encrypt(QByteArray text);
    QByteArray decrypt(QByteArray text);
    ~AesCrypto();
private:
    QByteArray aesCrypto(QByteArray text,CryptoType type);
    void genrateIvec(unsigned char * ivec);
private:
    Algorithm m_type;
    QByteArray m_key;
    const QMap<Algorithm,algorithmFunc> m_algorithms={
            { AES_ECB_128, EVP_aes_128_ecb },
            { AES_CBC_128, EVP_aes_128_cbc },
            { AES_CFB_128, EVP_aes_128_cfb128 },
            { AES_OFB_128, EVP_aes_128_ofb },
            { AES_CTR_128, EVP_aes_128_ctr },

            { AES_ECB_192, EVP_aes_192_ecb },
            { AES_CBC_192, EVP_aes_192_cbc },
            { AES_CFB_192, EVP_aes_192_cfb128 },
            { AES_OFB_192, EVP_aes_192_ofb },
            { AES_CTR_192, EVP_aes_192_ctr },

            { AES_ECB_256, EVP_aes_256_ecb },
            { AES_CBC_256, EVP_aes_256_cbc },
            { AES_CFB_256, EVP_aes_256_cfb128 },
            { AES_OFB_256, EVP_aes_256_ofb },
            { AES_CTR_256, EVP_aes_256_ctr },
    };
};

#endif // AESCRYPTP_H
