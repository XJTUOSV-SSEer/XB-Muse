#include "EnclaveUtils.h"
#include "CryptoEnclave_t.h"

#include "sgx_trts.h"
#include "sgx_tcrypto.h"

void printf( const char *fmt, ...)
{
    char buf[BUFSIZ] = {'\0'};
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, BUFSIZ, fmt, ap);
    va_end(ap);
    ocall_print_string(buf);
}

void print_bytes(uint8_t *ptr,uint32_t len) {  
    ocall_print_string_bytes((char *)ptr,len);
}

int aes_encrypt(unsigned char *plaintext, int plaintext_len,
                unsigned char *key, unsigned char *iv,
                int iv_len,unsigned char *ciphertext){
    unsigned char niv[iv_len];
    memcpy(niv,iv,iv_len);
    sgx_aes_ctr_encrypt(
        (sgx_aes_ctr_128bit_key_t*) key, plaintext, plaintext_len,
        niv, SGXSSL_CTR_BITS, ciphertext);
    return plaintext_len;
};

int aes_decrypt(unsigned char *ciphertext, int ciphertext_len,
                unsigned char *key, unsigned char *iv,
                int iv_len,unsigned char *plaintext){
    unsigned char niv[iv_len];
    memcpy(niv,iv,iv_len);
    sgx_aes_ctr_decrypt(
        (sgx_aes_ctr_128bit_key_t*) key,ciphertext,
        ciphertext_len,niv,
        SGXSSL_CTR_BITS, plaintext
    );
    return ciphertext_len;
};

void sha256_digest(unsigned char *plaintext, int plaintext_len,
                unsigned char *digest){
    sgx_sha256_hash_t sha256_hash;

    sgx_sha256_msg(plaintext,plaintext_len,&sha256_hash);
    memcpy(digest,&sha256_hash,DIGEST_SIZE);
};

unsigned int hmac_digest(unsigned char *plaintext, int plaintext_len,
                 unsigned char *key, int key_len,
                 unsigned char *digest){
    
    sgx_sha256_hash_t sha256_hash;

    sgx_sha256_msg(plaintext,plaintext_len,&sha256_hash);
    memcpy(digest,&sha256_hash,DIGEST_SIZE);
    return DIGEST_SIZE;

};

unsigned int key_derivation(unsigned char *plaintext, int plaintext_len,
                            unsigned char *key, int key_len,
                            unsigned char *digest){
    sgx_sha256_hash_t sha256_hash;

    sgx_sha256_msg(plaintext,plaintext_len,&sha256_hash);
    memcpy(digest,&sha256_hash,AES_BLOCK_SIZE);
    return AES_BLOCK_SIZE;
};