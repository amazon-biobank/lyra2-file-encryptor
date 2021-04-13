void addJsonElement(const char* elementName, const char* elementValue, FILE * jsonFileReference, bool isTheLast, size_t valueSize);

void beginJsonFile(FILE * jsonFileReference);

void endJsonGroup(FILE * jsonFileReference);

void addJsonElementName(const char* elementName, FILE * jsonFileReference);

void endElement(bool isTheLast, FILE * jsonFileReference);