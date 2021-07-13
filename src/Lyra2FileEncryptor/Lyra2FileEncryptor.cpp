#include "aesFile.h"
#include "LyraHash.h"
#include "FileAux.h"
#include "JsonEncryptionFormatter.h"
#include "base64Util.h"
#include "cypherString.h"

#include <json/value.h>
#include <json/json.h>
#include <json/writer.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include <string>

#define ENCRYPT 1
#define DECRYPT 0
#define SALT_SIZE 16
#define HASH_SIZE 64

std::string encryptString(std::string stringToEncrypt, std::string password){
    unsigned char* passowrdLyraHashed = (unsigned char*) malloc(HASH_SIZE * sizeof(unsigned char));
    char* randomSalt = (char*) malloc(sizeof(char) * (SALT_SIZE + 1));
    generateSalt(randomSalt, SALT_SIZE);
    size_t saltSize = 0;
    if (LyraHash(passowrdLyraHashed, (char *) password.c_str(), randomSalt, SALT_SIZE) != 0){
        printf("Couldn't hash the password.\n");
        return "";
    }

    // Cypher content
    std::string cypheredContent(cypherString(stringToEncrypt, passowrdLyraHashed));

    // encode salt
    char* saltBase64C = base64_encode(
        (unsigned char *) randomSalt,
        SALT_SIZE,
        &saltSize
    );
    std::string saltBase64(saltBase64C, saltSize);

    // Generate cypher json
    Json::Value jsonCypher;
    jsonCypher["hashing_alg"] = "Lyra2";
    jsonCypher["hashing_output_len"] = 256;
    jsonCypher["hashing_t_cost"] = 1;
    jsonCypher["hashing_mem_cost"] = 49152;
    jsonCypher["encrypt_alg"] = "AES-cfb-128";
    jsonCypher["salt"] = saltBase64;
    jsonCypher["encrypted_content"] = cypheredContent.c_str();

    std::string outputJson = jsonCypher.toStyledString();

    return outputJson;
}

std::string decryptString(std::string stringToDecrypt, std::string password, std::string saltString){
    // First hash the password with the salt.
    unsigned char* passowrdLyraHashed = (unsigned char*) malloc(HASH_SIZE * sizeof(unsigned char));
    const char *salt64Base = saltString.c_str();
    size_t saltSize = 0;
    unsigned char* salt = base64_decode(
        salt64Base,
        strlen(salt64Base),
        &saltSize
    );
    if (LyraHash(passowrdLyraHashed, (char *) password.c_str(), (char *) salt, SALT_SIZE) != 0){
        printf("Couldn't hash the password.\n");
        return "";
    }
    std::string cypheredString = decypherString(stringToDecrypt, passowrdLyraHashed);
    
    return cypheredString;
}

void encryptFile(std::string inputFilePath, std::string outputFilePath, std::string password){
    std::string stringToEncrypt = getFileContents(inputFilePath.c_str());

    std::string cypheredContent = encryptString(stringToEncrypt, password);

    std::ofstream decypheredFile(outputFilePath);
    decypheredFile << cypheredContent;
    decypheredFile.close();
}

std::string getDecryptedContentFromFile(std::string inputFilePath, std::string password){
    std::ifstream cypheredFile(inputFilePath);
    Json::Value inputJson;
    Json::Reader reader;
    reader.parse(cypheredFile, inputJson);

    std::string stringToDecypher = inputJson["encrypted_content"].asString();
    std::string passwordSalt = inputJson["salt"].asString();

    std::string decypheredString = decryptString(stringToDecypher, password, passwordSalt);

    cypheredFile.close();

    return decypheredString;
}

void decryptFile(std::string inputFilePath, std::string outputFilePath, std::string password){
    std::string decryptedContent = getDecryptedContentFromFile(inputFilePath, password);

    std::ofstream outputFile(outputFilePath);

    outputFile << decryptedContent;

    outputFile.close();
}
