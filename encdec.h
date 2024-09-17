#include "struct_header.h"

enc_out encode(unsigned char *in, unsigned int count);

void cpy_to_fp(FILE *fp, unsigned char *in, unsigned int in_size, unsigned char lst_fill_bit);

dec_out decode_file(FILE *fp, unsigned char bit_no);
