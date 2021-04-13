#include <openssl/aes.h>
#include <stdlib.h>
#include <stdio.h>

#include "Lyra2FileEncryptor.h"

int main(void){
    char password[] = "50ad41624c25e493aa1dc7f4ab32bdc5";
    char inputTestPath[] = "ibmUser.id";
    char encryptedTestPath[] = "encrypted.lock";
    char decryptedTestPath[] = "decrypted.pem";

    encryptFile(inputTestPath, encryptedTestPath, password);

    decryptFile(encryptedTestPath, decryptedTestPath, password);

    return 0;
}