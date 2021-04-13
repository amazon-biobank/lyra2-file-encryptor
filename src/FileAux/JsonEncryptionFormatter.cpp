#include <iostream>
#include <string.h>

void addJsonElement(const char* elementName, const char* elementValue, FILE * jsonFileReference, bool isTheLast, size_t valueSize) {
    fwrite("\"", 1, 1, jsonFileReference);
    fwrite(elementName, 1, strlen(elementName), jsonFileReference);
    fwrite("\": \"", 1, 4, jsonFileReference);
    fwrite(elementValue, 1, valueSize, jsonFileReference);
    fwrite("\"", 1, 1, jsonFileReference);
    if(!isTheLast){
        fwrite(",", 1, 1, jsonFileReference);
    }
}

void beginJsonFile(FILE * jsonFileReference){
    fwrite("{", 1, 1, jsonFileReference);
}

void endJsonGroup(FILE * jsonFileReference){
    fwrite("}", 1, 1, jsonFileReference);
}

void addJsonElementName(const char* elementName, FILE * jsonFileReference) {
    fwrite("\"", 1, 1, jsonFileReference);
    fwrite(elementName, 1, strlen(elementName), jsonFileReference);
    fwrite("\": \"", 1, 4, jsonFileReference);
}

void endElement(bool isTheLast, FILE * jsonFileReference){
    if(isTheLast){
        fwrite("\"", 1, 1, jsonFileReference);
    }
    else{
        fwrite("\",", 1, 2, jsonFileReference);
    }
}
