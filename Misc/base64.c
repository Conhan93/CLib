#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static const unsigned char base64_table[65] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";


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

static unsigned char* base64_hexstring_to_byte_array(char* string) {

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
    if(array == NULL) {
        if(pad) free(hexstring);
        return NULL;
    }

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

char* base64_encoder(unsigned char* bytes, size_t size) {
    const unsigned char *endpos = bytes + size;
    const unsigned char * input = bytes;

    size_t outlen = 4*((size + 2) / 3 ) + 1;

    unsigned char* output = malloc(outlen);
    if(!output) return NULL;

    unsigned char* index = output;

    while(endpos - input >= 3) {
        
        /*
        #define GET_FIRST_SEXTET(index, table, input) ((*index++ = table[(*input & 0b11111100) >> 2]))
        #define GET_SECOND_SEXTET(index, table, input) (*index++ = table[(*input & 0b00000011) << 4 | (*(input+1) & 0b11110000) >> 4])
        #define GET_THIRD_SEXTET(index, table, input) (*index++ = table[(*(input+1) & 0b00001111) << 2 | (*(input+2) & 0b11000000) >> 6 ])
        #define GET_FOURTH_SEXTET(index, table, input) (*index++ = table[*(input+2) & 0b00111111 ])
        */

       
        *index++ = base64_table[(*input & 0b11111100) >> 2];
        *index++ = base64_table[(*input & 0b00000011) << 4 | (*(input+1) & 0b11110000) >> 4];
        *index++ = base64_table[(*(input+1) & 0b00001111) << 2 | (*(input+2) & 0b11000000) >> 6 ];
        *index++ = base64_table[*(input+2) & 0b00111111 ];

        
        input += 3;
    }
    if(endpos - input) {
        *index++ = base64_table[*input >> 2];
        if(endpos - input == 1) {
            *index++ = base64_table[((*input & 0b00000011) << 4)];
            *index++ = '=';
        } else {
            *index++ = base64_table[((*input & 0b00000011) << 4) | (*(input+1) >> 4)];
            *index++ = base64_table[((*(input+1) & 0b00001111) << 2)];
        }
        *index++ = '=';
        
    }
    *index++ = '\0'; // close the string

    return output;


}
/**
 *  Converts a hex encoded string into base64
 *  @string: hex-encoded string to convert
 *  @return base64 encoded string : memory allocated,
 *  caller responsible for freeing memory!!!
 */
char* base64_encode_from_hex_string(char* string) {
    int len = strlen(string);
    unsigned char* byte_array = NULL;

    return (byte_array = base64_hexstring_to_byte_array(string)) ? base64_encoder(byte_array, len/2) : NULL;
}