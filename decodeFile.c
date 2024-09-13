
#include "struct_header.h"

dec_out decode_file(FILE *fp, unsigned char bit_no)
{
  unsigned char in;
  fread(&in, 1, 1, fp);
  //  printf("This is decode\n");
  short int flag = 0;
  unsigned int tot_bits = 0;
  unsigned long long int bits_to_read = 0;
  unsigned int inp_bits_read = bit_no;
  unsigned int inp_bytes = 0;
  unsigned long long int out = 2;
  while (!flag)
  {
    bits_to_read = out;
    out = 0;
    while (bits_to_read > 0)
    {
      if (inp_bits_read == 8)
      {
        inp_bytes++;
        fread(&in, sizeof(char), 1, fp);
        inp_bits_read = 0;
      }
      out = out << 1;
      out = (out | (1 & (in >> (8 - (inp_bits_read + 1)))));
      bits_to_read--;
      inp_bits_read++;
      tot_bits++;
    }
    // checking for flag bit
    // printf("before checking flag inp bits read :: %d\n", inp_bits_read);
    if (inp_bits_read == 8)
    {
      inp_bits_read = 0;
      inp_bytes++;
      fread(&in, sizeof(char), 1, fp);
    }
    flag = (1 & (in >> (8 - (inp_bits_read + 1))));
    inp_bits_read += 1;
    // printf("after checking flag inp bits read :: %d\n", inp_bits_read);
    tot_bits++;
  }
  bits_to_read = out;
  unsigned int m = ceil((double)out / 8);
  unsigned char *output = malloc(m);
  unsigned char bits_wrote = 0;
  unsigned int out_bytes = 0;
  while (flag && (bits_to_read > 0))
  {
    if (inp_bits_read == 8)
    {
      inp_bytes++;
      fread(&in, sizeof(char), 1, fp);
      inp_bits_read = 0;
    }
    if (bits_wrote == 8)
    {
      out_bytes++;
      bits_wrote = 0;
    }
    output[out_bytes] = output[out_bytes] << 1;
    output[out_bytes] = (output[out_bytes] | (1 & (in >> (8 - (inp_bits_read + 1)))));
    bits_to_read--;
    inp_bits_read++;
    tot_bits++;
    bits_wrote++;
    // printf("bitswrote from decoder %d\n", bits_wrote);
  }
  // while (bits_to_read < m)
  // {
  //   printf("%d ", output[bits_to_read]);
  //   bits_to_read++;
  // }
  // printf("\n");
  // printf("after loop bitswrote from decoder %d\n", bits_wrote);

  dec_out d1;
  d1.tot_bits_read = tot_bits;
  d1.lst_byte = bits_wrote;
  d1.decoded_string = output;
  return d1;
}
