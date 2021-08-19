#ifndef BASE16_H_INCLUDED
#define BASE16_H_INCLUDED

#include <stddef.h>

char* base16_bytearray_to_hexstring(unsigned char* bytearray, size_t len);
unsigned char* base16_hexstring_to_byte_array(char* string);

#endif