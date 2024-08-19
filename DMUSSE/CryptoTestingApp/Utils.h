#ifndef UTILS_H
#define UTILS_H

#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/hmac.h>
#include <openssl/rand.h>
#include <openssl/sha.h>
#include <openssl/err.h>
#include <string.h>
#include <iostream>
#include <iomanip>

#include "../common/data_type.h"
#include "../common/config.h"

// unsigned char *key = (unsigned char*) "0123456789123456";
// unsigned char *iv = (unsigned char*) "0123456789123456";

void printHexBytes(const std::string& str);

int aes_encrypt(unsigned char *plaintext, int plaintext_len,
                unsigned char *key, unsigned char *iv,
                unsigned char *ciphertext);

int aes_decrypt(unsigned char *ciphertext, int ciphertext_len,
                unsigned char *key, unsigned char *iv,
                unsigned char *plaintext);

void sha256_digest(unsigned char *plaintext, int plaintext_len,
                unsigned char *digest);

unsigned int hmac_digest(unsigned char *plaintext, int plaintext_len,
                 unsigned char *key, int key_len,
                 unsigned char *digest);

unsigned int key_derivation(unsigned char *plaintext, int plaintext_len,
                            unsigned char *key, int key_len,
                            unsigned char *digest);

#endif