#ifndef CRYPTO_H_INCLUDED
#define CRYPTO_H_INCLUDED

int crypto_fixedxor_hex(char* hex_in1, char* hex_in2, char* out);
int crypto_decrypt_singlebytexor_hex(char* encryptedhex, char* out, size_t lenout);
int crypto_encrypt_hex_repeating_xor(char* text,char* key, char* out, size_t outlen );

#endif