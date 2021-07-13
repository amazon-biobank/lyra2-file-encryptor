#include <string>
#include <openssl/aes.h>

std::string cypherString(std::string stringToCypher, unsigned char* password);
std::string decypherString(std::string stringToCypher, unsigned char* password);
