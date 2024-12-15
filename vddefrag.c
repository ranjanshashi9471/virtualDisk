#include "encdec.h"

int main()
{
        FILE *fp, *fp1;
        unsigned int totalFSize = 0, cpyMetaStart, cpyMetaBits;
        unsigned short count, tmpcount;
        fopen("vd", "rb+");
        fread(&count, 2, 1, fp);
        tmpcount = count;
        unsigned char bits = 0, tmpWriteStart = 0, tmpWriteBits = 0;
        unsigned char buff = 0;
        for (int i = 0; i < count; i++)
        {
                if (bits != 0)
                {
                        fseek(fp, -1, SEEK_CUR);
                }
                tmpWriteStart = ftell(fp);
                tmpWriteBits = bits;
                fread(&buff, 1, 1, fp);
                buff = ((buff >> (8 - (bits + 1))) & 1);
                bits++;
                bits %= 8;
                if (bits != 0)
                {
                        fseek(fp, -1, SEEK_CUR);
                }
                dec_out d1 = decode_file(fp, bits); // reading length of file
                bits = (d1.tot_bits_read - (8 - bits)) % 8;
                if (bits != 0)
                {
                        fseek(fp, -1, SEEK_CUR);
                }
                dec_out d2 = decode_file(fp, bits); // reading name of file
                bits = (d2.tot_bits_read - (8 - bits)) % 8;
                totalFSize += atoi(d1.decoded_string);
                if (buff)
                {
                        tmpcount--;
                        unsigned int writeBits = tmpWriteBits;
                        unsigned int writeStart = tmpWriteStart;
                        if(bits!=0)
                                fseek(fp, -1, SEEK_CUR);
                        cpyMetaStart = ftell(fp);
                        break;
                }
        }
        count = tmpcount;
        fseek(fp, 0, SEEK_SET);
        return 0;
}