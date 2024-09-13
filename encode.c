
#include "struct_header.h"

unsigned int calculate_bits_req(unsigned int count)
{
  unsigned int i = count;
  unsigned int size = 0;
  while (i >= 1)
  {
    size++;
    i = i >> 1;
  }
  return size;
}

void encode_helper(int count, int l, unsigned int *no_of_bits, unsigned int *num)
{
  if (count == 2)
    return;
  unsigned int size = calculate_bits_req(count);
  encode_helper(size, l - 1, no_of_bits, num);
  no_of_bits[l - 1] = size;
  num[l - 1] = count;
}

enc_out encode(unsigned char *in, unsigned int byteCount)
{
  enc_out output;
  // printf("This is encode, input is %s\n", in);
  unsigned int sum_of_bits_tot = 8 * byteCount;
  unsigned int count = sum_of_bits_tot;
  unsigned int levels = 0;
  while (count > 2)
  {
    levels++;
    count = calculate_bits_req(count);
    sum_of_bits_tot += count + 1;
  }
  unsigned int bytes_tot_req = ceil((double)sum_of_bits_tot / 8);
  unsigned int no_of_bits_to_write[levels];
  unsigned int what_to_write[levels];

  // storing what to write in how much bits
  encode_helper(8 * byteCount, levels, no_of_bits_to_write, what_to_write);
  unsigned int j = 0;
  unsigned char *out = (unsigned char *)malloc(bytes_tot_req);
  j = 0;
  int k = 0; // used for maintaining bytes written to out array
  // out[0] = (out[0] | ((0 | out[0]) << 7));
  // Encoding Starts Here
  unsigned int bitsWritten = 0, flagBits, flag = 0;

  while (j < levels)
  {
    if (bitsWritten == 8)
    {
      k++;
      bitsWritten = 0;
    }
    if (flag)
    {
      flag = 0;
      out[k] = (out[k] | (what_to_write[j]
                          << (8 - (no_of_bits_to_write[j] - flagBits))));
      bitsWritten += (no_of_bits_to_write[j] - flagBits);
      bitsWritten += 1;
      j++;
      flagBits = 0;
    }
    else if ((bitsWritten + no_of_bits_to_write[j] + 1) <= 8)
    {
      out[k] = (out[k] | what_to_write[j] << (8 - (no_of_bits_to_write[j] + bitsWritten)));
      bitsWritten += no_of_bits_to_write[j];
      bitsWritten += 1;
      j++;
    }
    else
    {
      out[k] = out[k] | (what_to_write[j] >> (no_of_bits_to_write[j] - (8 - bitsWritten)));
      flag = 1;
      flagBits = 8 - bitsWritten;
      bitsWritten = 8;
    }
  }

  if (bitsWritten == 8)
  {
    k++;
    bitsWritten = 0;
  }

  out[k] = ((1 | (out[k] >> (8 - bitsWritten)))) << (8 - bitsWritten);
  unsigned int l = 0;
  flag = 0;
  flagBits = 0;

  // writing the input data

  while (l < byteCount) // i shows the byte count of input
  {
    if (bitsWritten == 8)
    {
      k++;
      bitsWritten = 0;
    }
    if (flag)
    {
      flag = 0;
      out[k] = (out[k] | (in[l] << flagBits));
      l++;
      bitsWritten = 8 - flagBits;
      flagBits = 0;
    }
    else
    {
      out[k] = (out[k] | (in[l] >> bitsWritten));
      flag = 1;
      flagBits = 8 - bitsWritten;
      bitsWritten = 8;
    }
  }
  l = 0;
  // Check Encoded Sequence is correct or not l = 0;
  // while (l <= k)
  // {
  //   printf("%x ", out[l]);
  //   l++;
  // }
  // printf("\n");

  output.encoded_text = out;
  output.encoded_bits = sum_of_bits_tot;
  return output;
}