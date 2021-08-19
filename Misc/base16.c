#include "base16.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>



static int hexchar_to_value(char character) {
    if(character >= '0' && character <= '9')
        return (character - '0');
    else if (character >= 'A' && character <= 'F') 
        return (10 + (character - 'A'));
    else if (character >= 'a' && character <= 'f')
        return (10 + (character - 'a'));
    else
        return -1;
}
static void value_to_hexchar(unsigned char value, char* output) {
    char* base16_table = "0123456789ABCDEF";
    char result[3] = "";
 
    // high value
    result[0] = base16_table[(value & 0b11110000) >> 4];
    //low value
    result[1] = base16_table[value & 0b00001111];
    
    strncat(output, result, 2);
}
/**
 * Takes a hexstring and converts it into a byte array
 * @string: hexstring
 * @return returns a malloced byte array - caller responsible
 * for freeing the byte array!!!
 */
unsigned char* base16_hexstring_to_byte_array(char* string) {

    char* hexstring = string;

    if(!hexstring) return NULL;

    size_t len = strlen(hexstring);
    unsigned char pad = len % 2;
    if(pad) { // if uneven
        hexstring = malloc(strlen(string) + 1);
        if(!hexstring) return NULL;
    
        sprintf(hexstring, "0%s", string);
        len = strlen(hexstring);
    }
    
    unsigned char* array = calloc(len /2  , sizeof(unsigned char));
    if(array == NULL) return NULL;

    unsigned long index = 0;
    while(index < len) {
        char character = hexstring[index];
        int val = 0;
 
        if((val = hexchar_to_value(character)) < 0) {
            if(pad) free(hexstring);
            free(array);
            return NULL;
        }
       
        // index/2 rounds down, advances index everyother byte
        array[(index/2)] += val << (((index + 1) % 2) * 4);
        index++;
    }
    if(pad) free(hexstring);
    return array;
}
/**
 *  Converts a byte array into a hexadecimal string
 * @bytearray: a byte array
 * @len: the length or size of the array
 * @returns a malloced hexadecimal string
 * caller responsible for freeing string!!!
 */
char* base16_bytearray_to_hexstring(unsigned char* bytearray, size_t len) {
    const unsigned char* input = bytearray;
    const unsigned char* endpos = bytearray + len;

    size_t outlen = len * 2;

    char* output = malloc(outlen);
    if(!output) return NULL;
    
    strcpy(output, "");

    while(input - endpos) 
        value_to_hexchar(*input++, output);

    return output;
}