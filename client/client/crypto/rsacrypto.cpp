#include "rsacrypto.h"
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/bn.h>
#include "base64.h"

RsaCrypto::RsaCrypto(QObject *parent) : QObject(parent), m_pubKey(nullptr), m_priKey(nullptr)
{

}

RsaCrypto::RsaCrypto(QByteArray filename, RsaCrypto::KeyType type, QObject *parent)
    : QObject(parent), m_pubKey(nullptr), m_priKey(nullptr)
{
    BIO *bio=BIO_new_file(filename.data(),"rb");
    assert(bio!=NULL);
    if(type==PublicKey){
        PEM_read_bio_PUBKEY(bio,&m_pubKey,NULL,NULL);
    }else{
        PEM_read_bio_PrivateKey(bio,&m_priKey,NULL,NULL);
    }
    BIO_free(bio);
}

RsaCrypto::~RsaCrypto()
{
    if(m_pubKey){
        EVP_PKEY_free(m_pubKey);
    }
    if(m_priKey){
        EVP_PKEY_free(m_priKey);
    }
}

void RsaCrypto::parseStringToKey(QByteArray data, RsaCrypto::KeyType type)
{
    BIO *bio=BIO_new_mem_buf(data.data(),data.size());
    assert(bio!=NULL);
    if(type==PublicKey){
        PEM_read_bio_PUBKEY(bio,&m_pubKey,NULL,NULL);
    }else{
        PEM_read_bio_PrivateKey(bio,&m_priKey,NULL,NULL);
    }
    BIO_free(bio);
}

void RsaCrypto::generateRsaKey(RsaCrypto::KeyLength bits, QByteArray pub, QByteArray pri)
{
    EVP_PKEY_CTX *ctx=EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL);
    int ret= EVP_PKEY_keygen_init(ctx);
    assert(ret==1);

   ret=EVP_PKEY_CTX_set_rsa_keygen_bits(ctx,bits);
   assert(ret==1);

   ret=EVP_PKEY_keygen(ctx, &m_priKey);
   assert(ret==1);
   EVP_PKEY_CTX_free(ctx);

   BIO * bio=BIO_new_file(pri.data(), "wb");
   ret=PEM_write_bio_PrivateKey(bio, m_priKey,NULL,NULL,0,NULL,NULL);
   assert(ret);
   BIO_flush(bio);
   BIO_free(bio);

   bio=BIO_new_file(pub.data(), "wb");
   ret=PEM_write_bio_PUBKEY(bio,m_priKey);
   assert(ret);
   BIO_flush(bio);
   BIO_free(bio);
}

QByteArray RsaCrypto::pubKeyEncrypt(QByteArray data)
{
    EVP_PKEY_CTX *ctx=EVP_PKEY_CTX_new(m_pubKey,NULL);
    assert(ctx!=NULL);

    int ret= EVP_PKEY_encrypt_init(ctx);
    assert(ret==1);

    ret= EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_OAEP_PADDING);
    assert(ret==1);

    size_t outlen=0;
    ret=EVP_PKEY_encrypt(ctx, NULL, &outlen,
                         reinterpret_cast<const unsigned char *>(data.data()), data.size());
    assert(ret==1);
    unsigned char *out=new unsigned char[outlen];
    ret=EVP_PKEY_encrypt(ctx, out, &outlen,
                         reinterpret_cast<const unsigned char *>(data.data()), data.size());
    assert(ret==1);

    Base64 b64;
    QByteArray retStr=b64.encode(reinterpret_cast<char *>(out),outlen);

    EVP_PKEY_CTX_free(ctx);

    delete []out;
    return retStr;
}

QByteArray RsaCrypto::priKeyDecrypt(QByteArray data)
{
    Base64 b64;
    data=b64.decode(data.data(),data.size());
    EVP_PKEY_CTX *ctx=EVP_PKEY_CTX_new(m_priKey,NULL);
    assert(ctx!=NULL);

    int ret= EVP_PKEY_decrypt_init(ctx);
    assert(ret==1);

    ret= EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_OAEP_PADDING);
    assert(ret==1);

    size_t outlen=0;
    ret=EVP_PKEY_decrypt(ctx, NULL, &outlen,
                         reinterpret_cast<const unsigned char *>(data.data()), data.size());
    assert(ret==1);
    unsigned char *out=new unsigned char[outlen];
    ret=EVP_PKEY_decrypt(ctx, out, &outlen,
                         reinterpret_cast<const unsigned char *>(data.data()), data.size());
    assert(ret==1);

    QByteArray retStr(reinterpret_cast<char *>(out),outlen);

    EVP_PKEY_CTX_free(ctx);

    delete []out;
    return retStr;
}

QByteArray RsaCrypto::sign(QByteArray data, QCryptographicHash::Algorithm hash)
{
    //hash
    QCryptographicHash h(hash);
    h.addData(data);
    QByteArray md=h.result();

    EVP_PKEY_CTX *ctx=EVP_PKEY_CTX_new(m_priKey,NULL);
    assert(ctx!=NULL);

    int ret= EVP_PKEY_sign_init(ctx);
    assert(ret==1);

    ret= EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_PADDING);
    assert(ret==1);

    ret=EVP_PKEY_CTX_set_signature_md(ctx, m_hash.value(hash)());
    assert(ret==1);

    size_t outlen=0;
    ret=EVP_PKEY_sign(ctx, NULL, &outlen,
                         reinterpret_cast<const unsigned char *>(md.data()), md.size());
    assert(ret==1);
    unsigned char *out=new unsigned char[outlen];
    ret=EVP_PKEY_sign(ctx, out, &outlen,
                         reinterpret_cast<const unsigned char *>(md.data()), md.size());
    assert(ret==1);

    Base64 b64;
    QByteArray retStr=b64.encode(reinterpret_cast<char *>(out),outlen);

    EVP_PKEY_CTX_free(ctx);

    delete []out;
    return retStr;
}

bool RsaCrypto::verify(QByteArray sign, QByteArray data, QCryptographicHash::Algorithm hash)
{
    Base64 b64;
    sign=b64.decode(sign.data(),sign.size());
    //hash
    QCryptographicHash h(hash);
    h.addData(data);
    QByteArray md=h.result();

    EVP_PKEY_CTX *ctx=EVP_PKEY_CTX_new(m_pubKey,NULL);
    assert(ctx!=NULL);

    int ret= EVP_PKEY_verify_init(ctx);
    assert(ret==1);

    ret= EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_PADDING);
    assert(ret==1);

    ret=EVP_PKEY_CTX_set_signature_md(ctx, m_hash.value(hash)());
    assert(ret==1);

    ret=EVP_PKEY_verify(ctx,
                        reinterpret_cast<const unsigned char *>(sign.data()), sign.size(),
                        reinterpret_cast<const unsigned char *>(md.data()), md.size());

    EVP_PKEY_CTX_free(ctx);
    if(ret==1){
        return true;
    }
    return false;
}
