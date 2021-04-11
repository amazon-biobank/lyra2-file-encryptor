#include <openssl/aes.h>
#include <stdlib.h>
#include <stdio.h>

#include "aesFile.h"
#include "Lyra2.h"

int main(void){
    FILE * pTestFile;
    FILE * pEncryptedTestFile;
    FILE * pDecryptedTestFile;

    unsigned char password[] = "50ad41624c25e493aa1dc7f4ab32bdc5";

    pTestFile = fopen("ibmUser.id", "rb");
    if (pTestFile == NULL) {
        printf("File not found");
    }

    pEncryptedTestFile = fopen("encrypted.lock", "wb");
    aesFile(pTestFile, 1, pEncryptedTestFile, password);
    fclose(pEncryptedTestFile);
    fclose(pTestFile);

    pEncryptedTestFile = fopen("encrypted.lock", "rb");
    pDecryptedTestFile = fopen("decrypted.pem", "wb");

    aesFile(pEncryptedTestFile, 0, pDecryptedTestFile, password);

    fclose(pEncryptedTestFile);
    fclose(pDecryptedTestFile);

    return 0;
}