#ifndef RSACRYPTO_H
#define RSACRYPTO_H

#include <string>
#include "Hash.h"
#include <openssl/evp.h>
#include <map>

class RsaCrypto
{
public:
    enum KeyLength{
        BITS_1k=1024,
        BITS_2k=2048,
        BITS_3k=3072,
        BITS_4k=4096,
    };
    enum KeyType{
        PublicKey,
        PrivateKey
    };

    using funcPtr=const EVP_MD* (*)(void);

    explicit RsaCrypto();
    //构造对象并加载秘钥文件中的数据到内存中
    explicit RsaCrypto(std::string filename,KeyType type);
    ~RsaCrypto();
    //将秘钥字符串解析为秘钥类型
    void parseStringToKey(std::string data,KeyType type);
    //生成密钥对
    void generateRsaKey(KeyLength bits,std::string pub="public.pem",std::string pri="private.pem");
    //通过公钥进行加密
    std::string pubKeyEncrypt(std::string data);
    //通过私钥进行解密
    std::string priKeyDecrypt(std::string data);
    //数据签名
    std::string sign(std::string data,HashType hash=HashType::Sha256);
    //签名校验
    bool verify(std::string sign,std::string data,HashType hash=HashType::Sha256);
private:
    EVP_PKEY* m_pubKey;
    EVP_PKEY* m_priKey;
    std::map<HashType,funcPtr> m_hash={
        { HashType::Md4,      EVP_md4 },
        { HashType::Md5,      EVP_md5},
        { HashType::Sha1,     EVP_sha1 },
        { HashType::Sha224,   EVP_sha224 },
        { HashType::Sha256,   EVP_sha256 },
        { HashType::Sha384,   EVP_sha384 },
        { HashType::Sha512,   EVP_sha512 },
        { HashType::Sha3_224, EVP_sha3_224 },
        { HashType::Sha3_256, EVP_sha3_256 },
        { HashType::Sha3_384, EVP_sha3_384 },
        { HashType::Sha3_512, EVP_sha3_512 }
    };
};

#endif // RSACRYPTO_H
