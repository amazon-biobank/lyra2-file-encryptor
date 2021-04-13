#include <openssl/aes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <streambuf>
#include <iostream>

#include "base64Util.h"

void aesFileEncrypt(FILE * inputFilePointer, FILE * outputFilePointer, unsigned char * password){
    FILE * tempFile = fopen("tmp.encrypt", "wb");
    int bytes_read, bytes_written;
    unsigned char indata[AES_BLOCK_SIZE];
    unsigned char outdata[AES_BLOCK_SIZE];

    unsigned char ckey[16];
    memcpy(&ckey, &password[0], 16 * sizeof(unsigned char)) ;
    unsigned char ivec[16];
    memcpy(&ivec, &password[16], 16 * sizeof(unsigned char)) ;

    AES_KEY key;
    if (AES_set_encrypt_key(ckey, 128, &key) < 0){
        printf("\n Couldn't create AES key");
    }

    int num = 0;
    size_t output_length;

    while (1) {
        bytes_read = fread(indata, 1, AES_BLOCK_SIZE, inputFilePointer);

        AES_cfb128_encrypt(indata, outdata, bytes_read, &key, ivec, &num, AES_ENCRYPT);
        
        bytes_written = fwrite(outdata, 1, bytes_read, tempFile);

        if (bytes_read < AES_BLOCK_SIZE)
            break;
    }

    fclose(tempFile);

    std::ifstream t("tmp.encrypt");
    std::string encryptedContent((std::istreambuf_iterator<char>(t)),
                    std::istreambuf_iterator<char>());
    unsigned char *cypher64Base = new unsigned char[encryptedContent.length() + 1];
    memcpy(cypher64Base, encryptedContent.c_str(), encryptedContent.length());

    char *encryptedBase64ToStore = base64_encode((unsigned char *) cypher64Base, encryptedContent.length(), &output_length);

    fwrite(encryptedBase64ToStore, 1, output_length, outputFilePointer);
    remove("tmp.encrypt");
}

void aesFileDecrypt(FILE * inputFilePointer, FILE * outputFilePointer, unsigned char * password){
    int bytes_read, bytes_written;
    unsigned char indata[AES_BLOCK_SIZE];
    unsigned char outdata[AES_BLOCK_SIZE];

    unsigned char ckey[16];
    memcpy(&ckey, &password[0], 16 * sizeof(unsigned char)) ;
    unsigned char ivec[16];
    memcpy(&ivec, &password[16], 16 * sizeof(unsigned char)) ;

    AES_KEY key;
    if (AES_set_encrypt_key(ckey, 128, &key) < 0){
        printf("\n Couldn't create AES key");
    }

    int num = 0;

    while (1) {
        bytes_read = fread(indata, 1, AES_BLOCK_SIZE, inputFilePointer);

        AES_cfb128_encrypt(indata, outdata, bytes_read, &key, ivec, &num, AES_DECRYPT);
        
        bytes_written = fwrite(outdata, 1, bytes_read, outputFilePointer);

        if (bytes_read < AES_BLOCK_SIZE)
            break;
    }
}
