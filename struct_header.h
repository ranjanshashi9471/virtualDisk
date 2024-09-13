#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

typedef struct dec_out
{
        unsigned int tot_bits_read;
        unsigned char lst_byte;
        unsigned char *decoded_string;
} dec_out;

typedef struct enc_out
{
        unsigned char *encoded_text;
        unsigned int encoded_bits;
} enc_out;