#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static const unsigned char base64_table[65] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static const unsigned char base64_decode_table[] =
    { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 62, 63, 62, 62, 63, 52, 53, 54, 55,
56, 57, 58, 59, 60, 61,  0,  0,  0,  0,  0,  0,  0,  0,  1,  2,  3,  4,  5,  6,
7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,  0,
0,  0,  0, 63,  0, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51 };



/**
 *  Takes a byte array and encodes them as base-64 encoded string
 * @bytes: the byte array
 * @size: the size or length of the array
 * @return: base-64 encoded string - string is malloced
 * caller responsible for freeing memory!!
 */
char* base64_encoder(unsigned char* bytes, size_t size) {
    const unsigned char *endpos = bytes + size;
    const unsigned char * input = bytes;

    size_t outlen = 4*((size + 2) / 3 ) + 1;

    unsigned char* output = malloc(outlen);
    if(!output) return NULL;

    unsigned char* index = output;

    while(endpos - input >= 3) {

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
 * Decodes a base-64 encoded string into a byte array
 * @base_64: base-64 encoded string
 * @len_out: the size of the returned byte array
 * @return the decoded string as malloced unsigned pointer
 *  - array is malloced caller responsible for freeing memory! 
 */
unsigned char* base64_decoder(char* base_64, size_t* len_out) {
    
    size_t len = strlen(base_64);
    if(len % 4) return NULL;

    unsigned char* in = (unsigned char*)base_64;
    unsigned char* endpos = in + len;

    size_t size = (len / 4)*3;

    if(*(in+2) == '=') size--;
    if(*(in+3) == '=') size--;

    unsigned char* bytes = malloc(size);
    if(!bytes) return NULL;
    unsigned char* bytes_pos = bytes;

    while(endpos - in >= 4) {
            if(*(in+2) == '=' || *(in+3) == '=') break;

            unsigned char sextet_1 = base64_decode_table[*in];
            unsigned char sextet_2 = base64_decode_table[*(in+1)];
            unsigned char sextet_3 = base64_decode_table[*(in+2)];
            unsigned char sextet_4 = base64_decode_table[*(in+3)];

            *bytes_pos++ = ((sextet_1 << 2) | ((sextet_2 & 0b110000) >> 4));
            *bytes_pos++ = (((sextet_2 & 0b1111) << 4) | ((sextet_3 & 0b111100) >> 2 )) ;
            *bytes_pos++ = (((sextet_3 & 0b11) << 6) | (sextet_4 & 0b111111));

            in += 4;
    }

    if(*(in+2) == '=' && *(in+3) == '=') {
        unsigned char sextet_1 = base64_decode_table[*in];
        unsigned char sextet_2 = base64_decode_table[*(in+1)];

        *bytes_pos++ = ((sextet_1 << 2) | (sextet_2 & 0b00110000) >> 4);
    } else if(*(in+3) == '=') {
        unsigned char sextet_1 = base64_decode_table[*in];
        unsigned char sextet_2 = base64_decode_table[*(in+1)];
        unsigned char sextet_3 = base64_decode_table[*(in+2)];

        *bytes_pos++ = ((sextet_1 << 2) | ((sextet_2 & 0b110000) >> 4));
        *bytes_pos++ = (((sextet_2 & 0b1111) << 4) | ((sextet_3 & 0b111100) >> 2 )) ;
    }

    *len_out = size;
    
    return bytes;
}