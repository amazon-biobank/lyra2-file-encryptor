#include <iostream>
#include <fstream>

using namespace std;
size_t getFileSize(char const *filePath){
    ifstream in_file(filePath, ios::binary);
    in_file.seekg(0, ios::end);
    size_t file_size = in_file.tellg();
    return file_size;
}