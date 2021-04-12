# Lyra2 file encryptor

This repository implements a file encryptor with AES and uses Lyra2 as the password hashing algorithm

The Lyra2 implementation comes from this repository: [Leocalm Lyra](https://github.com/leocalm/Lyra).

## File hierarchy

```
repository/
├── build/
|    ├── objects/ 
|    └── OurCypher 
├── include/
|    ├── sse/
|    ├── LyraHash.h
|    └── aesFile.h  
├── src
|    ├── AES/
|    ├── Lyra2/
|    ├── HashWrapper/ 
|    └── main.cpp
└── Makefile
```

The Makefile will compile each c file (those in `src/Lyra2`) with gcc and each cpp file individually. Each one of these object files will be stored inside the `build/objects/` directory.

In the final step, the Makefile will link all those object files in a executable named `build/OurCypher`.

This project brings code from the Lyra2 implementation from the mentioned repository. The specific folder being copied to this implementation is the `sse`. It's cpp files are stored in `src/Lyra2/`. All .h files related to it are stored in `include/sse/`.

`src/AES/aesFile.cpp` is an AES openssl wrapper to facilitate it's use for a file encryption by providing a hashed and salted password and File descriptors of the file to encrypt or decrypt.

`src/HashWrapper/LyraHash.cpp` is a Lyra2 wrapper to facilitate it's use for a string hashing. It provides a function for providing a salt generator of a given value and calls the Lyra2 function.

The main.cpp in the moment is providing a test ride over these functionalities.

All .h files are inside include folder.

### Objective of this repository

- Create an easy to compile (for linux) library that will be installed to `usr/lib/` or `usr/local/lib/` to facilitate compilation with this file encryptor/decryptor by simply adding `-lLyra2Encryptor` as a linkable library to c/c++ projects

### How it works

User journey

1. Provide a arbitrary password string;
2. Provide a file;
3. (in decryption use case) provides salt used for it's encryption;
3. Specify if wants to encrypt or decrypt;
4. Obtains the resultant file;

Lib work for encryption

1. Generate a random string for salt;
2. Hash the password provided along with the generated salt with Lyra2 algorithm; (Not implemented yet)
3. Separates the first half of the hash to be AES key;
4. Separates the last half of the hash to be AES IV;
5. Encrypt file with the provided AES Key and IV;
6. Returns salt value;

Lib work for decryption

1. Hash the password provided along with the provided salt with Lyra2 algorithm; (Not implemented yet)
2. Separates the first half of the hash to be AES key;
3. Separates the last half of the hash to be AES IV;
4. Decrypts file with the provided AES Key and IV;

### Source code for Lyra, Lyra2 and yescrypt for GPU

####Lyra Specification
Lyra specifications can be found [here](http://eprint.iacr.org/2014/030).

####Lyra2 Specification
Lyra2 specifications can be found [here](http://eprint.iacr.org/2015/136).

