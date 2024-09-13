#include "encdec.h"

int main(int argc, char *argv[])
{
        if (argc < 3)
        {
                printf("Arguments missing\n");
                return 1;
        }
        FILE *fp;
        unsigned short fileno = atoi(argv[2]);
        if ((fp = fopen(argv[1], "rb+")) == NULL)
        {
                printf("Error opening virtual disk\n");
                return 1;
        }
        unsigned short count;
        fread(&count, sizeof(count), 1, fp);
        if (count < fileno)
        {
                printf("Wrong fileno given or file doesn't exists\n");
                return 1;
        }
        unsigned char bits = 0;
        unsigned char buff;
        for (int i = 0; i < fileno - 1; i++)
        {
                if (bits != 0)
                {
                        fseek(fp, -1, SEEK_CUR);
                }
                fread(&buff, 1, 1, fp);
                bits++;
                bits %= 8;
                if (bits != 0)
                {
                        fseek(fp, -1, SEEK_CUR);
                }
                dec_out d1 = decode_file(fp, bits); // reading length of file

                bits = (d1.lst_byte) % 8;
                if (bits != 0)
                {
                        fseek(fp, -1, SEEK_CUR);
                }
                dec_out d2 = decode_file(fp, bits); // reading name of file

                bits = (d2.lst_byte) % 8;
        }
        if (bits != 0)
        {
                fseek(fp, -1, SEEK_CUR);
        }
        fread(&buff, 1, 1, fp);
        buff = buff | (((buff >> (8 - (bits + 1))) | 1) << (8 - (bits + 1)));
        fseek(fp, -1, SEEK_CUR);
        fwrite(&buff, 1, 1, fp);
        fclose(fp);
        return 0;
}