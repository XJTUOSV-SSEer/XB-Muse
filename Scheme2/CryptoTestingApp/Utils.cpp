#include "Utils.h"
// #include <vector>
// #include <iostream>
 
// using std::string;
// using std::vector;

//此函数做调试用
void printHexBytes(const std::string& str) {
    for (char c : str) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(static_cast<unsigned char>(c));
    }
    std::cout << std::dec << std::endl;
}


int aes_encrypt(unsigned char *plaintext, int plaintext_len,
                unsigned char *key, unsigned char *iv,
                unsigned char *ciphertext) {
    EVP_CIPHER_CTX *ctx;

    int len = 0;

    int ciphertext_len;

    /* Create and initialise the context */
    ctx = EVP_CIPHER_CTX_new();

    /* Initialise the encryption operation. */
    EVP_EncryptInit_ex(ctx, EVP_aes_128_ctr(), NULL, key, iv);

    /* Encrypt the message */
    EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len);
    ciphertext_len = len;

    /* Finalise the encryption */
    EVP_EncryptFinal_ex(ctx, ciphertext + len, &len);
    ciphertext_len += len;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    return ciphertext_len;
}

int aes_decrypt(unsigned char *ciphertext, int ciphertext_len,
                unsigned char *key, unsigned char *iv,
                unsigned char *plaintext) {
    EVP_CIPHER_CTX *ctx;

    int len = 0;

    int plaintext_len;

    /* Create and initialise the context */
    ctx = EVP_CIPHER_CTX_new();

    /* Initialise the decryption operation. */
    EVP_DecryptInit_ex(ctx, EVP_aes_128_ctr(), NULL, key, iv);

    /* decrypt the message */
    EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len);
    plaintext_len = len;

    /* Finalise the encryption */
    EVP_DecryptFinal_ex(ctx, plaintext + len, &len);
    plaintext_len += len;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    return plaintext_len;
}

void sha256_digest(unsigned char *plaintext, int plaintext_len,
                  unsigned char *digest) {
    /* Create and initialise the context */
    SHA256_CTX ctx;
    SHA256_Init(&ctx);

    /* compute the digest */
    SHA256_Update(&ctx, plaintext, plaintext_len);

    /* Finalise the digest */
    SHA256_Final(digest, &ctx);
}

unsigned int hmac_digest(unsigned char *plaintext, int plaintext_len,
                 unsigned char *key, int key_len,
                 unsigned char *digest) {


    sha256_digest(plaintext,plaintext_len,digest);

    // HMAC_CTX *ctx;

    // unsigned int len;

    // /* Create and initialise the context */
    // ctx = HMAC_CTX_new();

    // /* Initialise the decryption operation. */
    // HMAC_Init_ex(ctx, key, key_len, EVP_sha256(), NULL);

    // /* compute the digest */
    // HMAC_Update(ctx, plaintext, plaintext_len);

    // /* Finalise the digest */
    // HMAC_Final(ctx, digest, &len);

    // /* Clean up */
    // HMAC_CTX_free(ctx);

    return DIGEST_SIZE;
}

unsigned int key_derivation(unsigned char *plaintext, int plaintext_len,
                         unsigned char *key, int key_len,
                         unsigned char *digest) {

    unsigned char buffer[DIGEST_SIZE];
    sha256_digest(plaintext,plaintext_len,buffer);
    memcpy(digest,buffer,AES_BLOCK_SIZE);

    // HMAC_CTX *ctx;

    // unsigned int len;

    // /* Create and initialise the context */
    // ctx = HMAC_CTX_new();

    // /* Initialise the decryption operation. */
    // HMAC_Init_ex(ctx, key, key_len, EVP_md5(), NULL);

    // /* compute the digest */
    // HMAC_Update(ctx, plaintext, plaintext_len);

    // /* Finalise the digest */
    // HMAC_Final(ctx, digest, &len);

    // /* Clean up */
    // HMAC_CTX_free(ctx);

    return AES_BLOCK_SIZE;
}

string send_http(boost::asio::io_service &io_service,boost::asio::ip::tcp::resolver::iterator endpoint_iterator,string request){
    // 连接到服务器
    boost::asio::ip::tcp::socket socket(io_service);
    // cout<<6<<endl;
    boost::asio::connect(socket, endpoint_iterator);

    // 发送 HTTP 请求报文
    // cout<<7<<endl;
    boost::asio::write(socket, boost::asio::buffer(request));
    // cout<<8<<endl;

    // 读取响应报文
    boost::asio::streambuf response_buf;
    boost::system::error_code error;
    size_t bytes_transferred = 0;
    std::string response;

    // 分段读取响应报文
    do {
        bytes_transferred = boost::asio::read(socket, response_buf, boost::asio::transfer_at_least(1), error);
        response.append(boost::asio::buffers_begin(response_buf.data()), boost::asio::buffers_begin(response_buf.data()) + bytes_transferred);
        response_buf.consume(bytes_transferred);
    } while (!error);

    if (error != boost::asio::error::eof) {
        std::cout << "Error: " << error.message() << std::endl;
        socket.close();
        return "Error";
    }

    socket.close();

    return response;
}