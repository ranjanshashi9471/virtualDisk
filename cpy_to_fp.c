#include <stdio.h>

void cpy_to_fp(FILE *fp, unsigned char *in, unsigned int in_size, unsigned char lst_fill_bit)
{
        if (lst_fill_bit >= 8)
        {
                fprintf(stderr, "Invalid lst_fill_bit value\n");
                return;
        }

        unsigned int i = 0;
        unsigned char bitsWritten = lst_fill_bit;
        unsigned char bitsRead = 0;
        unsigned char out = 0;

        // If last byte is partially filled, read it
        if (lst_fill_bit != 0)
        {
                if (fread(&out, 1, 1, fp) != 1)
                {
                        perror("Error reading file");
                        return;
                }
                fseek(fp, -1, SEEK_CUR);
        }

        unsigned flag = 0;
        unsigned flagBits = 0;

        while (i < in_size)
        {
                if (bitsWritten == 8)
                {
                        if (fwrite(&out, 1, 1, fp) != 1)
                        {
                                perror("Error writing file");
                                return;
                        }
                        out = 0;
                        bitsWritten = 0;
                }

                if (bitsRead == 8)
                {
                        i++;
                        bitsRead = 0;
                }

                if (flag)
                {
                        flag = 0;
                        out = (out | (in[i] << flagBits));
                        i++;
                        bitsWritten = 8 - flagBits;
                        flagBits = 0;
                        bitsRead = flagBits;
                }
                else if (bitsWritten == 0 && bitsRead == 0)
                {
                        out = in[i];
                        bitsRead = 8;
                        bitsWritten = 8;
                }
                else
                {
                        out = (out | (in[i] >> bitsWritten));
                        flag = 1;
                        flagBits = 8 - bitsWritten;
                        bitsWritten = 8;
                        bitsRead = flagBits;
                }
        }

        if (bitsWritten != 0)
        {
                if (fwrite(&out, 1, 1, fp) != 1)
                {
                        perror("Error writing file");
                }
        }
}
