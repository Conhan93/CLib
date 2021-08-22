#ifndef BASE64_H_INCLUDED
#define BASE64_H_INCLUDED

char* base64_encoder(unsigned char* bytes, size_t size);
unsigned char* base64_decoder(char* base_64, size_t* len_out);

#endif