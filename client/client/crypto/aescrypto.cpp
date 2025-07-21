#include "aescrypto.h"

AesCrypto::AesCrypto(AesCrypto::Algorithm algorithm, QByteArray key, QObject *parent)
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

QByteArray AesCrypto::encrypt(QByteArray text)
{
    return aesCrypto(text,ENCRYPTO);
}

QByteArray AesCrypto::decrypt(QByteArray text)
{
    return aesCrypto(text,DECRYPTO);
}

AesCrypto::~AesCrypto()
{

}

QByteArray AesCrypto::aesCrypto(QByteArray text, AesCrypto::CryptoType type)
{
    unsigned char ivec[AES_BLOCK_SIZE];
    AesCrypto::genrateIvec(ivec);
    EVP_CIPHER_CTX* CTX=EVP_CIPHER_CTX_new();
    assert(CTX);
    int ret=EVP_CipherInit_ex(CTX,
                      m_algorithms.value(m_type)(),
                      NULL,
                      reinterpret_cast<unsigned char*>(m_key.data()),
                      ivec,
                      type);
    assert(ret);

    int len=text.size()+1;
    if(len%AES_BLOCK_SIZE){
        len=(len/AES_BLOCK_SIZE+1)*AES_BLOCK_SIZE;
    }
    unsigned char *out=new unsigned char[len];
    int outlen=0,totalLen=0;
    ret=EVP_CipherUpdate(CTX, out,&outlen, reinterpret_cast<unsigned char*>(text.data()), text.size());
    totalLen+=outlen;
    assert(ret);

    ret=EVP_CipherFinal_ex(CTX, out+outlen, &outlen);
    totalLen+=outlen;
    assert(ret);

    QByteArray outText(reinterpret_cast<char *>(out),totalLen);
    EVP_CIPHER_CTX_free(CTX);
    delete []out;
    return outText;
}

void AesCrypto::genrateIvec(unsigned char *ivec)
{
    QCryptographicHash hs(QCryptographicHash::Md5);
    hs.addData(m_key);
    std::string res=hs.result().toStdString();
    for(int i=0;i<AES_BLOCK_SIZE;i++){
        ivec[i]=res.at(i);
    }
}




