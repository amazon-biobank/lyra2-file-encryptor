#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <cerrno>

using namespace std;
string getFileContents(const char* filename)
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