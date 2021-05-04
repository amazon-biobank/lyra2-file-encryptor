#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <cerrno>

using namespace std;
size_t getFileSize(char const *filePath){
    ifstream in_file(filePath, ios::binary);
    in_file.seekg(0, ios::end);
    size_t file_size = in_file.tellg();
    return file_size;
}

string get_file_contents(const char* filename)
{
    FILE* fp = fopen(filename, "rb");
    if (fp)
    {
        string contents;
        fseek(fp, 0, SEEK_END);
        contents.resize(ftell(fp));
        rewind(fp);
        fread(&contents[0], 1, contents.size(), fp);
        fclose(fp);
        return(contents);
    }
    throw(errno);
}