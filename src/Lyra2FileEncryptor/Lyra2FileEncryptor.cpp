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

void encryptFile(std::string inputFilePath, std::string outputFilePath, std::string password){
    FILE * inputFile = fopen(inputFilePath.c_str(), "rb");
    FILE * outputFile = fopen(outputFilePath.c_str(), "wb");

    size_t outputSize;

    unsigned char* passowrdLyraHashed = (unsigned char*) malloc(HASH_SIZE * sizeof(unsigned char));
    char* randomSalt = (char*) malloc(sizeof(char) * (SALT_SIZE + 1));
    
    generateSalt(randomSalt, SALT_SIZE);

    if (LyraHash(passowrdLyraHashed, (char *) password.c_str(), randomSalt, SALT_SIZE) != 0){
        printf("Couldn't hash the password.\n");
        return;
    }

    char* saltBase64;
    saltBase64 = base64_encode((unsigned char *) randomSalt, SALT_SIZE, &outputSize);

    beginJsonFile(outputFile);
    addJsonElementName("encrypted_content", outputFile);
    aesFileEncrypt(inputFile, outputFile, passowrdLyraHashed);
    endElement(false, outputFile);
    addJsonElement("hashing_alg", "Lyra2", outputFile, false, strlen("Lyra2"));
    addJsonElement("hashing_output_len", "256", outputFile, false, strlen("256"));
    addJsonElement("hashing_t_cost", "1", outputFile, false, strlen("21"));
    addJsonElement("hashing_mem_cost", "49152", outputFile, false, strlen("4906"));
    addJsonElement("encrypt_alg", "AES-cfb-128", outputFile, false, strlen("AES-cfb-128"));
    addJsonElement("salt", saltBase64, outputFile, true, outputSize);
    endJsonGroup(outputFile);

    fclose(inputFile);
    fclose(outputFile);

    free(passowrdLyraHashed);
    free(randomSalt);
}

void decryptFile(std::string inputFilePath, std::string outputFilePath, std::string password){
    FILE * outputFile = fopen(outputFilePath.c_str(), "wb");

    unsigned char* passowrdLyraHashed = (unsigned char*) malloc(HASH_SIZE * sizeof(unsigned char));
    unsigned char* salt;
    unsigned char* encryptedBytes;
    size_t cypherSize = 0;
    size_t saltSize = 0;
    
    std::ifstream file(inputFilePath);
    Json::Value inputJson;
    Json::Reader reader;
    Json::FastWriter fastWriter;

    reader.parse(file, inputJson);

    file.close();

    // transform inputJson in char*:
    std::string saltString = fastWriter.write(inputJson["salt"]);

    // remove \"
    saltString = saltString.substr(1, saltString.size() - 3);
    const char *salt64Base = saltString.c_str();

    salt = base64_decode(
        salt64Base,
        strlen(salt64Base),
        &saltSize);
    
    std::ofstream out("tmp.decrypt");
    
    std::string encryptedContent = fastWriter.write(inputJson["encrypted_content"]);
    // remove \"
    encryptedContent = encryptedContent.substr(1, encryptedContent.size() - 3);
    const char *cypher64Base = encryptedContent.c_str();

    encryptedBytes = base64_decode(
        cypher64Base,
        strlen(cypher64Base),
        &cypherSize);

    FILE * tmpFileEncrypt = fopen("tmp.decrypt", "wb");
    fwrite(encryptedBytes, 1, cypherSize, tmpFileEncrypt);
    fclose(tmpFileEncrypt);

    if (LyraHash(passowrdLyraHashed, (char *) password.c_str(), (char *) salt, SALT_SIZE) != 0){
        printf("Couldn't hash the password.\n");
        return;
    }
    FILE * tmpBytes = fopen("tmp.decrypt", "rb");
    
    aesFileDecrypt(tmpBytes, outputFile, passowrdLyraHashed);

    fclose(outputFile);
    fclose(tmpBytes);

    free(passowrdLyraHashed);
    free(salt);

    remove ("tmp.decrypt");
}

std::string getDecryptedContentFromFile(std::string inputFilePath, std::string password){
    decryptFile(inputFilePath, "tmp.getContent", password);

    std::string decryptedContent = getFileContents("tmp.getContent");

    char *contentOutput = new char[decryptedContent.length() + 1];
    strcpy(contentOutput, decryptedContent.c_str());

    remove("tmp.getContent");

    return contentOutput;
}

std::string encryptString(std::string stringToEncrypt, std::string password){
    unsigned char* passowrdLyraHashed = (unsigned char*) malloc(HASH_SIZE * sizeof(unsigned char));
    char* randomSalt = (char*) malloc(sizeof(char) * (SALT_SIZE + 1));
    generateSalt(randomSalt, SALT_SIZE);
    size_t saltSize = 0;
    if (LyraHash(passowrdLyraHashed, (char *) password.c_str(), randomSalt, SALT_SIZE) != 0){
        printf("Couldn't hash the password.\n");
        return "";
    }

    passowrdLyraHashed[HASH_SIZE] = '\0';
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
