#include "base64.h"
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/buffer.h>

Base64::Base64()
{

}

std::string Base64::encode(std::string data)
{
    return encode(data.data(),data.size());
}

std::string Base64::encode(const char *data, int length)
{
    BIO* b64=BIO_new(BIO_f_base64());
    BIO* mem=BIO_new(BIO_s_mem());
    BIO_push(b64,mem);

    BIO_write(b64,data,length);
    BIO_flush(b64);

    BUF_MEM *ptr;
    BIO_get_mem_ptr(b64,&ptr);
    std::string str(ptr->data,ptr->length);
    BIO_free_all(b64);
    return str;
}

std::string Base64::decode(std::string data)
{
    return decode(data.data(),data.size());
}

std::string Base64::decode(const char *data, int length)
{
    BIO* b64=BIO_new(BIO_f_base64());
    BIO* mem=BIO_new(BIO_s_mem());
    BIO_push(b64,mem);

    BIO_write(mem,data,length);
    char *buf=new char[length];
    int ret=BIO_read(b64,buf,length);
    std::string str(buf,ret);
    BIO_free_all(b64);
    delete []buf;
    return str;
}
