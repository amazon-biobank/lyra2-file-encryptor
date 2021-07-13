#include <openssl/aes.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <string>
#include <iostream>

#include "Lyra2FileEncryptor.h"

int main(int argc, char *argv[ ]){
    std::string command;
    std::string inputPath;
    std::string outputPath;
    std::string password;
    std::string text;
    std::string salt;
    std::string fileMode;

    char *decryptedContent;

    switch (argc) {
        case 2:
            if (strcmp(argv[1], "--help") == 0) {
                printf("Usage: \n");
                printf("\t encrypt -f input-path output-path password\n");
                printf("\t decrypt -f input-path output-path password\n");
                printf("\t encrypt password \"text-to-cypher\"\n");
                printf("\t decrypt password salt cyphered-text\n");
                printf("Inputs:\n");
                printf(" - -f: activate file enc/decryption mode\n");
                printf(" - salt: password salt used for the encryption of a given cypher (available only in console mode)\n");
                printf(" - text-to-cypher: plain text to be encrypted via console mode. Must use double quotes around them and escape \" in text\n");
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
        case 4:
            command = std::string(argv[1]);
            password = std::string(argv[2]);
            text = std::string(argv[3]);

            if (strcmp(command.c_str(), "encrypt") == 0){
                std::string encryptedString = encryptString(text, password);
                std::cout << encryptedString << std::endl;
            }
            else if(strcmp(command.c_str(), "decrypt") == 0){
                std::cout << "Missing salt.\nTry --help\".\n";
                return 0;
            }
            else{
                std::cout << "Invalid options.\nTry --help\".\n";
                return 0;
            }
            break;
        case 5:
            command = std::string(argv[1]);
            password = std::string(argv[2]);
            salt = std::string(argv[3]);
            text = std::string(argv[4]);

            if (strcmp(command.c_str(), "encrypt") == 0){
                std::cout << "Too many arguments.\nTry --help\".\n";
                return 0;
            }
            else if(strcmp(command.c_str(), "decrypt") == 0){
                std::string decryptedString = decryptString(text, password, salt);
                std::cout << decryptedString << std::endl;
            }
            else{
                std::cout << "Invalid options.\nTry --help\".\n";
                return 0;
            }
            break;
        case 6:
            command = std::string(argv[1]);
            fileMode = std::string(argv[2]);
            inputPath = std::string(argv[3]);
            outputPath = std::string(argv[4]);
            password = std::string(argv[5]);

            if (fileMode.compare("-f") != 0){
                std::cout << "Invalid options.\nTry --help\".\n";
                return 0;
            }

            if (command.compare("encrypt") == 0){
                encryptFile(inputPath, outputPath, password);
                std::cout << "Succesfully encrypted file. Output file is " << outputPath << std::endl;
            }
            else if(command.compare("decrypt") == 0){
                decryptFile(inputPath, outputPath, password);
                std::cout << "Succesfully decrypted file. Output file is " << outputPath << std::endl;
            }
            else{
                std::cout << "Invalid options.\nTry --help\".\n";
                return 0;
            }
            break;

        default:
            std::cout << "Invalid options.\nTry --help\".\n";
            return 0;
    }

    return 0;
}