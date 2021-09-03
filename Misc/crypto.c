#include "base16.h"
#include "base64.h"
#include <string.h>
#include <stdlib.h>
#include <float.h>
#include <ctype.h>

#include <stdio.h>

#define BYTE_SIZE 8


static int crypto_letter_frequency_english(char* string);
static int crypto_string_mostly_not_letters(char* string, float noise_ratio);
static int crypto_score_common_words(char* string);
static int get_hamming_distance(char* first_text, size_t len1, char* second_text, size_t len2);

/**
 *  Performs XOR on two hex strings and outputs the result string in hex
 *  
 */
int crypto_fixedxor_hex(char* hex_in1, char* hex_in2, char* out) {

    unsigned char* bytearray1 = base16_hexstring_to_byte_array(hex_in1);
    unsigned char* bytearray2 = base16_hexstring_to_byte_array(hex_in2);

    size_t len1 = strlen(hex_in1)/2;
    size_t len2 = strlen(hex_in2)/2;

    const unsigned char* endpos1 = bytearray1 + len1;
    const unsigned char* endpos2 = bytearray2 + len2;

    size_t len_result = 0;

    if((endpos1 - endpos2) > 0)
        len_result = len1;
    else
        len_result = len2;

    unsigned char* resultbytes = malloc(len_result);
    if(resultbytes == NULL) {
        free(bytearray1);
        free(bytearray2);
        return -1;
    }
    
    unsigned char* index = resultbytes;

    while((bytearray1 - endpos1) && (bytearray2 - endpos2)) 
        *index++ = *bytearray1++ ^ *bytearray2++;

    free(bytearray1);
    free(bytearray2);

    char* res = base16_bytearray_to_hexstring(resultbytes, len_result);

    strcpy(out, res);

    free(res);
    free(resultbytes);

    return 1;
}

int crypto_decrypt_singlebytexor_hex(char* encryptedhex, char* out, size_t lenout) {
    size_t len = strlen(encryptedhex)/2;

    unsigned char* bytes_in = base16_hexstring_to_byte_array(encryptedhex);
    const unsigned char* bytes_in_endpos = bytes_in + len;


    int high_score = 0;
    for(unsigned char key = 0 ; key < 255 ; key++) {
        unsigned char* result = malloc(len + 1);
        if(!result) {
            free(bytes_in);
            return -1;
        }

        unsigned char* rindex = result;
        unsigned char* index = bytes_in;

        while(bytes_in_endpos - index)
            *rindex++ =  *index++ ^ key ;
        
        *rindex = '\0';
        char* restr = result; 

        if(crypto_string_mostly_not_letters(restr, 20.0f)) {
            
            //crypto_letter_frequency_english(restr);
            int score = crypto_score_common_words(restr);

            if(score > high_score) {
                high_score = score;
                strncpy(out, result, lenout);
            }
                
        }
            
        

        free(result);
    }

    free(bytes_in);

    return high_score;
}
int crypto_encrypt_hex_repeating_xor(char* text,char* key, char* out, size_t outlen ) {
    size_t textlen = strlen(text);
    unsigned char *textbytes = text;
    const unsigned char* text_endpos = textbytes + textlen;


    const unsigned char* key_start = key;
    const unsigned char* key_end = key + strlen(key);
    unsigned char* key_pos = key_start;

    unsigned char* encrypted = malloc(textlen);
    if(!encrypted) return -1;
    unsigned char* index = encrypted;


    while(text_endpos - textbytes) {
        if(!(key_end - key_pos))
            key_pos = key_start;

        *index++ =  *textbytes++ ^ *key_pos++;

    }
    
    char* resstr = base16_bytearray_to_hexstring(encrypted, textlen);
    strcpy(out, resstr);

    free(resstr);
    free(encrypted);
    return 1;

}
// WIP
/**
 *  from cryptopals challenge set 1 - challenge nr 6
 * 
 *     create a function to get the hamming distance between two strings. - check
 * 
 *     write a function to get a sample of keysizes with the lowest normalized
 *     hamming distance from a range of keysize blocks of cyphertext.
 * 
 *     transpose cypher into blocks of keysizes and decrypt as if it was a
 *     single byte xor encryption.
 * 
 *     
 *     
 */
int crypto_decrypt_repeating_xor(char* in, char* out, size_t outlen) {

    float lowest_ham_dist_normalized = FLT_MAX, ham_dist_result_normalized = 0.0F;

    for(int key_size = 1 ; key_size < 40 ; key_size++) {
        unsigned char* cypherblock = in;
        ham_dist_result_normalized = 
            get_hamming_distance(cypherblock, key_size, cypherblock + key_size, key_size);
        if(ham_dist_result_normalized < lowest_ham_dist_normalized) {
            lowest_ham_dist_normalized = ham_dist_result_normalized;
        }
    }
    return 1;
}
//TODO : give it a proper name
static int crypto_string_mostly_not_letters(char* string, float noise_ratio) {
    size_t len = strlen(string);

    unsigned long count = 0;
    char* index = string;

    while(*index != '\0') {
        if(!isalnum(*index) && !isspace(*index))
            count++;
        index++;
    }

    float ratio = (float)count / len;

    if((ratio*100) > noise_ratio)
        return 0;
    else
        return 1; 
}
static int crypto_score_common_words(char* string) {
    char common_words[][8] = {" the ", " of ", " and ", " a ", " to ",
    " in ", " is ", " you ", " that ", " it ", " he ", " was ", " for ",
    " on ", " are ", " as ", " with " , " they ", " I ", " by "};

    size_t len = strlen(string);

    int points = 0;
    for(int index = 0 ; index < 20 ; index++)
        if(strstr(string, common_words[index]))
            points++;

    return points;
}
// WIP
static int crypto_letter_frequency_english(char* string) {
    size_t len = strlen(string);

    unsigned char * input = string;
    const unsigned char* endpos = input + len;

    char letters[] = "etaoinsrhdlucmfywq";
    int count[256] = {0};
    float frequencies[256] = {0.0f};

    while(endpos - input) 
        count[*input++]++;
    

    for(int index = 0 ; index < 256 ; index++)
        if(count[index])
        {
            frequencies[index] = (float)count[index]/len;        
        }
    int points = 0;
    for(int index = 0 ; index + 1 < 19 ; index++) {
        if(
            (frequencies[letters[index]] + frequencies[toupper(letters[index])])
             >
              (frequencies[letters[index+1]]+frequencies[toupper(letters[index+1])])
              )
            points++; 
        
    }

    return points;

} 
static int get_hamming_distance(char* first_text, size_t len1, char* second_text, size_t len2) {
    unsigned char* first = first_text;
    unsigned char* first_end = first + len1;

    unsigned char* second = second_text;
    unsigned char* second_end = second + len2;

    int hamming_distance = 0;

    while((first_end - first) && (second_end - second)) {
        for(unsigned bit = 0 ; bit < BYTE_SIZE ; bit++) {
            unsigned first_bit = (*first & (1UL << bit) );
            unsigned second_bit = (*second & (1UL << bit) );

            if(first_bit ^ second_bit)
                hamming_distance++;
        }
        
        first++; second++;
    }

    hamming_distance += (first_end - first) ? (first_end - first) * 8 :
        (second_end - second) ? (second_end - second) * 8 : 0;


    return hamming_distance;
}