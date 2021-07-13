# Lyra2 file encryptor

This repository implements a file encryptor with AES and uses Lyra2 as the password hashing algorithm. It's developed as a vcpkg packet.

The Lyra2 implementation comes from this repository: [Leocalm Lyra](https://github.com/leocalm/Lyra).

## Pre-requisites

- openssl
- jsoncpp

## How to install

### VCPKG

First, clone a vcpkg repository with this repo entry:

- Oficial vcpkg from microsoft;
- Our vcpkg fork;

Then bootstrap vcpkg (if not bootstraped already):

- `./bootstrap-vcpkg.sh` on linux;
- `bootstrap-vcpkg.bat` on windows;

Then integrate vcpkg (if not already integrated):

- `vcpkg integrate install`

Then install dependencies (if not already installed):

In windows:

- `vcpkg install jsoncpp:x86-windows-static`; (x86)
- `vcpkg install jsoncpp:x64-windows-static`; (x64)

- `vcpkg install openssl:x86-windows-static`; (x86)
- `vcpkg install openssl:x64-windows-static`; (x64)

In linux:
- `vcpkg install jsoncpp`;
- `vcpkg install openssl`;

Finally, install our package:

In windows:

- `vcpkg install lyra2-file-encryptor:x86-windows-static`; (x86)
- `vcpkg install lyra2-file-encryptor:x64-windows-static`; (x64)

In linux:
- `vcpkg install lyra2-file-encryptor`;

Integrating with your CMake project:

- `cmake -G "Ninja" -B build -DCMAKE_TOOLCHAIN_FILE="C:\\path\\to\\vcpkg\\scripts\\buildsystems\\vcpkg.cmake"`
- `cmake -G "Ninja" -B build -DCMAKE_TOOLCHAIN_FILE="~/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake"`

## Own build

## CMake

First, simply call this to setup the build system

`cmake -G "Ninja" -B cmake-build-dir -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE="path_to/vcpkg/scripts/buildsystems/vcpkg.cmake" -DCMAKE_INSTALL_PREFIX="path/you/want/to/install/"`

Then, let it build

`cmake --build cmake-build-dir`

Finally, specify where to install (use sudo if the destiny is somewhere protected)

`sudo cmake --install cmake-build-dir`

## Makefile for ubuntu (old version)

### Install binary

`sudo make install-bin`

check if was succesfull installed: `lyra2encryptor --help`

You can uninstall with `sudo make uninstall-bin`.

#### what does it does?

The installation will copy the resultant binary on `/usr/local/bin/` path in yout linux file system.

### Install linkable library

`sudo make install-lib`

You can uninstall with `sudo make uninstall-lib`.

#### what does it does?

The installation will create a directory called `Lyra2Encryptor/` in the `/usr/local/lib/` and `/usr/local/include/`. The `Lyra2FileEncryptor.h` will be copied to `/usr/local/include/Lyra2FileEncryptor.h` and `Lyra2FileEncryptor.a` will be copied to `/usr/local/lib/Lyra2FileEncryptor.a`.

#### How to link this library to your c/cpp project.

1. (optional): you can simply copy the installed header file inside the include files inside your project instead.

2. (optional): Add Include directive in your code.

- using from installed path: `#include "Lyra2FileEncryptor.h"`.

- using from copied path: `#include "your_include_folder/Lyra2FileEncryptor.h"`.

3. Specify in linkable libraries in compilation the path for the library. (You can also copy the lybrary installed to a lib folder in the project)

(How do i do that? `-lLyra2Encryptor`)

4. Link with openssl and jsoncpp libraries. (Why? because this lib depends on these dependencies)

Example code:

```
#include "Lyra2FileEncryptor.h"

int main(void){
    char password[] = "passwordExample";
    char fileToEncryptPath[] = "inputTest.pem";
    char encryptedFilePath[] = "outputTest.lock";
    char decryptedFilePath[] = "decryptedTest.pem";
    char *decryptedContent;

    // encrypt file example
    encryptFile(fileToEncryptPath, encryptedFilePath, password);

    // decrypt file example
    decryptFile(encryptedFilePath, decryptedFilePath, password);

    // get decrypted string example
    decryptedContent = getDecryptedContentFromFile(encryptedFilePath, password);

    return 0;
}

```

Compilation command example if using l2fe from installed root:

`c++ src/main.c -o testProgram -L/usr/lib/jsoncpp -lLyra2FileEncryptor -lcrypto -lssl -ljsoncpp`

Compilation command example if using l2fe from installed root:

`c++ src/main.c -o testProgram -I./include -L./lib -L/usr/lib/jsoncpp -lLyra2FileEncryptor -lcrypto -lssl -ljsoncpp`

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

