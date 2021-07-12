#include <openssl/aes.h>
#include <string>
#include <cstring>
#include <iostream>

#include "base64Util.h"

#define PASSWORD_SIZE 64
#define KEY_SIZE_BYTES 32
#define KEY_SIZE_BITS 256

std::string cypherString(std::string stringToCypher, unsigned char* password){
    // generate a AES_KEY for encryption
    unsigned char ckey[KEY_SIZE_BYTES];
    unsigned char ivec[KEY_SIZE_BYTES];

    memcpy(&ckey, &password[0],              KEY_SIZE_BYTES * sizeof(unsigned char)) ;
    memcpy(&ivec, &password[KEY_SIZE_BYTES], KEY_SIZE_BYTES * sizeof(unsigned char)) ;

    AES_KEY key;
    AES_set_encrypt_key(ckey, KEY_SIZE_BITS, &key);
    // AES_KEY generated

    // Pad input string to be multiple of 4 and be decoded
    // while(stringToCypher.size() % 4 != 0)
    //     stringToCypher.append(" ");
    // input padded

    // Decode input string in base64
    size_t decodedInputLength;
    unsigned char* decodedInputString = (unsigned char*) malloc(stringToCypher.size());  // No malloc, because base64_decode already mallocs

    decodedInputString = (unsigned char*) stringToCypher.c_str();
    decodedInputLength = stringToCypher.size();

    // decoded input string

    // Cypher blocks
    unsigned char* encryptedBytes = (unsigned char*) malloc(decodedInputLength);
    
    unsigned char indata[AES_BLOCK_SIZE];   // 16 bytes
    unsigned char outdata[AES_BLOCK_SIZE];  // 16 bytes
    size_t bytesRead;
    int charactersEncrypted = 0;
    int startIndex = 0;

    std::cout << "startIndex: " << startIndex << std::endl;
    std::cout << "decodedInputLength: " << decodedInputLength << std::endl;

    bool isEncryptionEnd = (startIndex >= decodedInputLength);
    
    std::cout << "isEncryptionEnd: " << isEncryptionEnd << std::endl;

    while(!isEncryptionEnd){
        // Transfering bytes to indata buffer
        if (decodedInputLength < startIndex + AES_BLOCK_SIZE) {
            bytesRead = decodedInputLength - startIndex;
            memcpy(indata, &decodedInputString[startIndex], bytesRead);
        }
        else {
            bytesRead = AES_BLOCK_SIZE;
            memcpy(indata, &decodedInputString[startIndex], bytesRead);
        }

        // DEBUG (TO CYPHER BYTES)
        std::cout << "indata buffer: ";
        for (int i = 0; i < bytesRead; i++)
            printf(" %u ", indata[i]);
        std::cout << std::endl;
        // DEBUG

        // Transferred bytes to indata buffer
        AES_cfb128_encrypt(
            indata,                 // unsigned char * holds block to cypher
            outdata,                // unsigned char * outputs block cyphered
            bytesRead,              // size_t the number of bytes in indata
            &key,                   // AES_KEY* for converted key
            ivec,                   // unsigned char* for IV
            &charactersEncrypted,   // int, number of characters cyphered
            AES_ENCRYPT             // AES operation macro. AES_ENCRYPT | AES_DECRYPT
        );

        // DEBUG (CYPHERED BYTES)
        std::cout << "outdata buffer: ";
        for (int i = 0; i < bytesRead; i++)
            printf(" %u ", outdata[i]);
        std::cout << std::endl;
        // DEBUG

        memcpy(&encryptedBytes[startIndex], outdata, bytesRead * sizeof(unsigned char));
        startIndex += bytesRead;
        isEncryptionEnd = (startIndex >= decodedInputLength);
    }
    // Blocks cyphered
    // Encode encryption
    size_t encodedEncryptionLength;
    char* encodedEncryption = base64_encode(
        encryptedBytes,             // unsigned char*
        decodedInputLength,         // size_t
        &encodedEncryptionLength    // size_t*
    );

    std::string resultEncryption(encodedEncryption, encodedEncryptionLength);
    // Encryption encoded

    return resultEncryption;
}

std::string decypherString(std::string stringToDecypher, unsigned char* password){
    // generate a AES_KEY for encryption
    unsigned char ckey[KEY_SIZE_BYTES];
    unsigned char ivec[KEY_SIZE_BYTES];

    memcpy(&ckey, &password[0],              KEY_SIZE_BYTES * sizeof(unsigned char)) ;
    memcpy(&ivec, &password[KEY_SIZE_BYTES], KEY_SIZE_BYTES * sizeof(unsigned char)) ;

    AES_KEY key;
    AES_set_encrypt_key(ckey, KEY_SIZE_BITS, &key);
    // AES_KEY generated

    // Decode input string in base64
    size_t decodedInputLength;
    unsigned char* decodedInputString;  // No malloc, because base64_decode already mallocs
    decodedInputString = base64_decode(
        stringToDecypher.c_str(),   // const char*
        stringToDecypher.size(),    // size_t
        &decodedInputLength         // size_t*
    );
    
    // decoded input string

    // Cypher blocks
    unsigned char* decryptedBytes = (unsigned char*) malloc(decodedInputLength * sizeof(unsigned char));
    unsigned char indata[AES_BLOCK_SIZE];   // 16 bytes
    unsigned char outdata[AES_BLOCK_SIZE];  // 16 bytes
    size_t bytesRead;
    int charactersDecrypted = 0;
    int startIndex = 0;

    bool isDecryptionEnd = (startIndex >= decodedInputLength);
    
    while(!isDecryptionEnd){
        // Transfering bytes to indata buffer
        if (decodedInputLength < startIndex + AES_BLOCK_SIZE) {
            bytesRead = decodedInputLength - startIndex;
            memcpy(indata, &decodedInputString[startIndex], bytesRead);
        }
        else {
            bytesRead = AES_BLOCK_SIZE;
            memcpy(indata, &decodedInputString[startIndex], bytesRead);
        }
        // Transferred bytes to indata buffer

        AES_cfb128_encrypt(
            indata,                 // unsigned char * holds block to cypher
            outdata,                // unsigned char * outputs block cyphered
            bytesRead,              // size_t the number of bytes in indata
            &key,                   // AES_KEY* for converted key
            ivec,                   // unsigned char* for IV
            &charactersDecrypted,   // int, number of characters cyphered
            AES_DECRYPT             // AES operation macro. AES_ENCRYPT | AES_DECRYPT
        );
        memcpy(&decryptedBytes[startIndex], outdata, bytesRead * sizeof(unsigned char));

        startIndex += bytesRead;
        isDecryptionEnd = (startIndex >= decodedInputLength);
    }
    // Blocks cyphered

    // Encode encryption
    std::string resultDecryption((char *) decryptedBytes, decodedInputLength);
    // Encryption encoded

    return resultDecryption;
}
