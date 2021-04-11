
#include <openssl/aes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// encrypt = 1;
// decrypt = 0
void aesFile(FILE * inputFilePointer, int encrypt, FILE * outputFilePointer, unsigned char * password){
    int bytes_read, bytes_written;
    unsigned char indata[AES_BLOCK_SIZE];
    unsigned char outdata[AES_BLOCK_SIZE];

    unsigned char ckey[16];
    memcpy(&ckey, &password[0], 16 * sizeof(unsigned char)) ;
    unsigned char ivec[16];
    memcpy(&ivec, &password[16], 16 * sizeof(unsigned char)) ;

    AES_KEY key;
    if (AES_set_encrypt_key(ckey, 128, &key) < 0){
        printf("\n Couldn't create AES key");
    }

    int num = 0;

    while (1) {
        bytes_read = fread(indata, 1, AES_BLOCK_SIZE, inputFilePointer);

        AES_cfb128_encrypt(indata, outdata, bytes_read, &key, ivec, &num, encrypt);
        
        bytes_written = fwrite(outdata, 1, bytes_read, outputFilePointer);

        if (bytes_read < AES_BLOCK_SIZE)
            break;
    }
}
