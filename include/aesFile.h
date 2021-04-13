#include <openssl/aes.h>
#include <stdio.h>

void aesFileEncrypt(FILE * inputFilePointer, FILE * outputFilePointer, unsigned char * password);
void aesFileDecrypt(FILE * inputFilePointer, FILE * outputFilePointer, unsigned char * password);