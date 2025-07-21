#include "AesCrypto.h"
#include <assert.h>
#include "Hash.h"

AesCrypto::AesCrypto(AesCrypto::Algorithm algorithm, std::string key)
{
    switch(algorithm){
    case AES_ECB_128:
    case AES_CBC_128:
    case AES_CFB_128:
    case AES_OFB_128:
    case AES_CTR_128:
        assert(key.size()==16);
        break;
    case AES_ECB_192:
    case AES_CBC_192:
    case AES_CFB_192:
    case AES_OFB_192:
    case AES_CTR_192:
        assert(key.size()==24);
        break;
    case AES_ECB_256:
    case AES_CBC_256:
    case AES_CFB_256:
    case AES_OFB_256:
    case AES_CTR_256:
        assert(key.size()==32);
        break;
    }

    m_type=algorithm;
    m_key=key;
}

std::string AesCrypto::encrypt(std::string text)
{
    return aesCrypto(text,ENCRYPTO);
}

std::string AesCrypto::decrypt(std::string text)
{
    return aesCrypto(text,DECRYPTO);
}

AesCrypto::~AesCrypto()
{

}

std::string AesCrypto::aesCrypto(std::string text, AesCrypto::CryptoType type)
{
    unsigned char ivec[AES_BLOCK_SIZE];
    AesCrypto::genrateIvec(ivec);
    EVP_CIPHER_CTX* CTX=EVP_CIPHER_CTX_new();
    assert(CTX);
    int ret=EVP_CipherInit_ex(CTX,
                      m_algorithms.at(m_type)(),
                      NULL,
                      reinterpret_cast<const unsigned char*>(m_key.data()),
                      ivec,
                      type);
    assert(ret);

    int len=text.size()+1;
    if(len%AES_BLOCK_SIZE){
        len=(len/AES_BLOCK_SIZE+1)*AES_BLOCK_SIZE;
    }
    unsigned char *out=new unsigned char[len];
    int outlen=0,totalLen=0;
    ret=EVP_CipherUpdate(CTX, out,&outlen, reinterpret_cast<const unsigned char*>(text.data()), text.size());
    totalLen+=outlen;
    assert(ret);

    ret=EVP_CipherFinal_ex(CTX, out+outlen, &outlen);
    totalLen+=outlen;
    assert(ret);

    std::string outText(reinterpret_cast<char *>(out),totalLen);
    EVP_CIPHER_CTX_free(CTX);
    delete []out;
    return outText;
}

void AesCrypto::genrateIvec(unsigned char *ivec)
{
    Hash hs(HashType::Md5);
    hs.addData(m_key);
    std::string res=hs.result(Hash::Type::Binary);
    for(int i=0;i<AES_BLOCK_SIZE;i++){
        ivec[i]=res.at(i);
    }
}




