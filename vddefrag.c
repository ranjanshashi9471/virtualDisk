#include "encdec.h"

int main()
{
        FILE *fp, *fp1;
        unsigned int vdSize;
        unsigned int totalFSize = 0, metaDataSize = 0;
        unsigned int writeMetaStart, writeFileStart, cpyMStart, cpyFileStart;
        fp = fopen("vd", "rb+");
        fp1 = fopen("vd", "rb+");

        unsigned short count;
        unsigned short tmpcount = count;
        fread(&count, 2, 1, fp);
        unsigned char bits = 0;
        unsigned char buff;
        for (int i = 0; i < count; i++)
        {
                if (bits != 0)
                {
                        fseek(fp, -1, SEEK_CUR);
                }
                fread(&buff, 1, 1, fp);
                buff = ((buff >> (8 - (bits + 1))) & 1);
                bits++;
                bits %= 8;
                if (bits != 0)
                {
                        fseek(fp, -1, SEEK_CUR);
                }
                unsigned tmp = ftell(fp);
                dec_out d1 = decode_file(fp, bits); // reading length of file
                bits = d1.lst_byte % 8;
                if (bits != 0)
                {
                        fseek(fp, -1, SEEK_CUR);
                }
                dec_out d2 = decode_file(fp, bits); // reading name of file
                bits = (d2.lst_byte) % 8;
                metaDataSize += d1.tot_bits_read;
                metaDataSize += d2.tot_bits_read;
                totalFSize += atoi(d1.decoded_string);
                if (buff)
                {
                        writeMetaStart = tmp;
                        tmpcount--;
                }
        }
}