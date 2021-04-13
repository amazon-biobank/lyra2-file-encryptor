char* base64_encode(unsigned char *data,
                    size_t input_length,
                    size_t *output_length);

unsigned char* base64_decode(const char *data,
                             size_t input_length,
                             size_t *output_length);