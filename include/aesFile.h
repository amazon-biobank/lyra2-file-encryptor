#include <openssl/aes.h>

void aesFile(FILE * inputFilePointer, int encrypt, FILE * outputFilePointer, unsigned char * password);