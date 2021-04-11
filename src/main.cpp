#include <openssl/aes.h>
#include <stdlib.h>
#include <stdio.h>

#include "aesFile.h"
#include "LyraHash.h"

#define SALT_SIZE 16

int main(void){
    FILE * pTestFile;
    FILE * pEncryptedTestFile;
    FILE * pDecryptedTestFile;

    char password[] = "50ad41624c25e493aa1dc7f4ab32bdc5";
    unsigned char* passowrdLyraHashed = (unsigned char*) malloc(64 * sizeof(unsigned char));

    char* randomSalt = (char*) malloc(sizeof(char) * (SALT_SIZE + 1));
    
    generateSalt(randomSalt, SALT_SIZE);

    printf("RANDOM SALT:\n");
    for (int i = 0; i < SALT_SIZE; i++){
        printf("%c", randomSalt[i]);
    }
    printf("\n");

    int lyraReturn = LyraHash(
        passowrdLyraHashed, 
        password, 
        randomSalt);

    printf("Lyra Hashed Password:\n");
    for (int i = 0; i < 64; i++){
        printf("%c", passowrdLyraHashed[i]);
    }
    printf("\n");

    pTestFile = fopen("ibmUser.id", "rb");
    if (pTestFile == NULL) {
        printf("File not found");
    }

    pEncryptedTestFile = fopen("encrypted.lock", "wb");
    aesFile(pTestFile, 1, pEncryptedTestFile, (unsigned char *) password);
    fclose(pEncryptedTestFile);
    fclose(pTestFile);

    pEncryptedTestFile = fopen("encrypted.lock", "rb");
    pDecryptedTestFile = fopen("decrypted.pem", "wb");

    aesFile(pEncryptedTestFile, 0, pDecryptedTestFile, (unsigned char *) password);

    fclose(pEncryptedTestFile);
    fclose(pDecryptedTestFile);

    return 0;
}