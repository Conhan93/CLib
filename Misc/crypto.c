#include "base16.h"
#include "base64.h"
#include <string.h>
#include <stdlib.h>

#include <ctype.h>

#include <stdio.h>


static int crypto_letter_frequency_english(char* string);
static int crypto_string_mostly_not_letters(char* string, float noise_ratio);
static int crypto_score_common_words(char* string);

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