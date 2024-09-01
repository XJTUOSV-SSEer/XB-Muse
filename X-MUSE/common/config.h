#ifndef CONFIG_H
#define CONFIG_H

#define AES_BLOCK_SIZE 16
#define DIGEST_SIZE 32

//这里要求 GGM_SIZE <= 2 ^ GGM_LEVEL
#define GGM_SIZE 1918
#define GGM_LEVEL 11
#define HASH_SIZE 13

#define SHA256_DIGEST_LENGTH 32
#define SGXSSL_CTR_BITS 128

#define KEY_LEN 16
#define IV_LEN 16

#endif
