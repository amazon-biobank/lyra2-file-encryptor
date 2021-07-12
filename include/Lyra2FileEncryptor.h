#include <string>

void encryptFile(std::string inputFilePath, std::string outputFilePath, std::string password);
void decryptFile(std::string inputFilePath, std::string outputFilePath,  std::string password);
std::string getDecryptedContentFromFile(std::string inputFile, std::string password);
std::string encryptString(std::string stringToEncrypt, std::string password);
std::string decryptString(std::string stringToDecrypt, std::string password, std::string salt);