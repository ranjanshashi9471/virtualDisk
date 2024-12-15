#include "struct_header.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

dec_out decode_file(FILE *fp, unsigned char bit_no)
{
  if (bit_no > 8)
  {
    fprintf(stderr, "Invalid bit_no value\n");
    exit(EXIT_FAILURE);
  }

  unsigned char in;
  if (fread(&in, 1, 1, fp) != 1)
  {
    perror("Error reading file");
    exit(EXIT_FAILURE);
  }

  short int flag = 0;
  unsigned int tot_bits = 0;
  unsigned long long int bits_to_read = 0;
  unsigned int inp_bits_read = bit_no;
  unsigned int inp_bytes = 0;
  unsigned long long int out = 2;

  // Decode header
  while (!flag)
  {
    bits_to_read = out;
    out = 0;
    while (bits_to_read > 0)
    {
      if (inp_bits_read == 8)
      {
        inp_bytes++;
        if (fread(&in, sizeof(char), 1, fp) != 1)
        {
          perror("Error reading file");
          exit(EXIT_FAILURE);
        }
        inp_bits_read = 0;
      }
      out = out << 1;
      out = (out | (1 & (in >> (8 - (inp_bits_read + 1)))));
      bits_to_read--;
      inp_bits_read++;
      tot_bits++;
    }

    // Check for flag bit
    if (inp_bits_read == 8)
    {
      inp_bytes++;
      if (fread(&in, sizeof(char), 1, fp) != 1)
      {
        perror("Error reading file");
        exit(EXIT_FAILURE);
      }
      inp_bits_read = 0;
    }
    flag = (1 & (in >> (8 - (inp_bits_read + 1))));
    inp_bits_read += 1;
    tot_bits++;
  }

  // Decode body
  bits_to_read = out;
  unsigned int m = ceil((double)out / 8);
  unsigned char *output = malloc(m);
  if (!output)
  {
    perror("Memory allocation failed");
    exit(EXIT_FAILURE);
  }

  unsigned char bits_wrote = 0;
  unsigned int out_bytes = 0;
  while (flag && (bits_to_read > 0))
  {
    if (inp_bits_read == 8)
    {
      inp_bytes++;
      if (fread(&in, sizeof(char), 1, fp) != 1)
      {
        perror("Error reading file");
        free(output);
        exit(EXIT_FAILURE);
      }
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
  }

  // Populate output structure
  dec_out d1;
  d1.tot_bits_read = tot_bits;
  d1.lst_byte = bits_wrote;
  d1.decoded_string = output;
  return d1;
}
