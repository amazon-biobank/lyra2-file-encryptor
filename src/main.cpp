#include <openssl/aes.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "Lyra2FileEncryptor.h"

int main(int argc, char *argv[ ]){
    char *command;
    char *inputPath;
    char *outputPath;
    char *password;

    char *decryptedContent;

    switch (argc) {
        case 2:
            if (strcmp(argv[1], "--help") == 0) {
                printf("Usage: \n");
                printf("\t encrypt: input-path output-path password\n");
                printf("\t decrypt: input-path output-path password\n");
                printf("Inputs:\n");
                printf(" - input-path: the file to encrypt or decrypt\n");
                printf(" - output-path: the resultant file encrypted or decrypted\n");
                printf(" - password: password used to encrypt or decrypt file\n");
                printf("\n");
                return 0;
            } else {
                printf("Invalid options.\nFor more information, try --help\".\n");
                return 0;
            }
            break;

        case 5:
            command = argv[1];
            inputPath = argv[2];
            outputPath = argv[3];
            password = argv[4];
            if (strcmp(command, "encrypt") == 0){
                encryptFile(inputPath, outputPath, password);
                printf("Succesfully encrypted file. Output file is %s\n", outputPath);
            }
            else if(strcmp(command, "decrypt") == 0){
                decryptFile(inputPath, outputPath, password);
                printf("Succesfully decrypted file. Output file is %s\n", outputPath);
            }
            else{
                printf("Invalid options.\nTry --help\".\n");
                return 0;
            }
            break;

        default:
            printf("Invalid options.\nTry --help\".\n");
            return 0;
    }

    return 0;
}