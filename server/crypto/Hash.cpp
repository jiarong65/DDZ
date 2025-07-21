#include "Hash.h"
#include <assert.h>

Hash::Hash(HashType type){
    m_type=type;
    m_ctx=EVP_MD_CTX_new();
    assert(m_ctx!=NULL);

    int ret=EVP_DigestInit_ex(m_ctx,m_hash.at(m_type)(),NULL);
    assert(ret==1);
}

Hash::~Hash(){
    if(m_ctx!=NULL){
        EVP_MD_CTX_free(m_ctx);
    }
}

void Hash::addData(std::string data){
    addData(data.data(),data.size());
}

void Hash::addData(const char *data,int length){
    int ret=EVP_DigestUpdate(m_ctx, data, length);
    assert(ret==1);
};

std::string Hash::result(Type type){
    unsigned int len;
    unsigned char out[HashLength.at(m_type)];
    int ret=EVP_DigestFinal_ex(m_ctx,out, &len);
    assert(ret==1);
    if(type==Type::Hex){
        char res[len*2];
        for(int i=0;i<len;i++){
            sprintf(&res[2*i],"%02x",out[i]);
        }
        return std::string(res,len*2);
    }

    return std::string(reinterpret_cast<char *>(out),len);
}