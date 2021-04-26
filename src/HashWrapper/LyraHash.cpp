#include <string.h>
#include <stdlib.h>
#include <iostream>

#define NROWS 49152
#define NCOLS 256

extern "C" int PHS(void*, size_t, const void*, size_t, const void*, size_t, unsigned int, unsigned int);

void generateSalt(char* returnSalt, int saltSize){
    char saltChar;
    srand (time(NULL));
    for (int i; i < saltSize; i++){
        saltChar = (char) (rand());
        returnSalt[i] = saltChar;
    }
    returnSalt[saltSize] = '\0';
}

int LyraHash(unsigned char *hashOutput, char *input, char *salt, unsigned int saltLength){
    unsigned int hashDesiredLength = 64;

    unsigned int t_cost = 1;
    unsigned int m_cost = NROWS;

    unsigned int inputLength = strlen(input);

    return PHS(
        hashOutput,         // unsigned char * (tamanho 64); vazio
        hashDesiredLength,  // unsigned int (64)
        input,              // char * (tamanho 32)
        inputLength,        // unsigned int (32)
        salt,               // char * (tamanho 16)
        saltLength,         // unsigned int (16)
        t_cost,             // t_cost = 1
        m_cost              // m_cost = NROWS = 49152; Config de intel i5
    );
}
