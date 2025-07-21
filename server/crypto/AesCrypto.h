#ifndef AESCRYPTO_H
#define AESCRYPTO_H


#include<string>
#include <openssl/evp.h>
#include<map>
#include <openssl/aes.h>



class AesCrypto
{
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

    AesCrypto(Algorithm algorithm,std::string key);

    //加解密
    std::string encrypt(std::string text);
    std::string decrypt(std::string text);
    ~AesCrypto();
private:
    std::string aesCrypto(std::string text,CryptoType type);
    void genrateIvec(unsigned char * ivec);
private:
    Algorithm m_type;
    std::string m_key;
    const std::map<Algorithm,algorithmFunc> m_algorithms={
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
