#include <string.h>
#include <random>

#include "Lyra2.h"

#define NROWS 49152
#define NCOLS 256

unsigned char* generateSalt(int saltSize){
    unsigned char salt[saltSize];
    unsigned char saltChar;
    for (int i; i < saltSize; i++){
        saltChar = (unsigned char) rand();
        salt[i] = saltChar;
    }
    return salt;
}

//tCost = 1; nRows = 49152; nCols = 256; nThreads = 1; nRoundsSponge = 1; bSponge = 12; sponge = 0;
int LyraHash(char *input, char *salt){
    unsigned int hashDesiredLength = 64;
    unsigned char hashOutput[hashDesiredLength];

    unsigned int t_cost = 1;
    unsigned int m_cost = NROWS;

    unsigned int inputLength = strlen(input);
    unsigned int saltLength = strlen(salt);

    return PHS(
        hashOutput,
        hashDesiredLength, 
        input,
        inputLength,
        salt,  
        saltLength, 
        t_cost, 
        m_cost
    );
}
