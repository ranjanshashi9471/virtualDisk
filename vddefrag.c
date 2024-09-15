#include "encdec.h"

int main()
{
        FILE *fp, *fp1;
        unsigned int vdSize;
        unsigned int totalFSize = 0, metaDataSize = 0;
        unsigned char dflag = 0, ndflag = 0;
        unsigned int writeMetaStart, writeMetabits, writeFileStart, cpyMStart, cpyMBitStart, cpyMetaSize, cpyFileStart, cpyFileSize;
        fp = fopen("vd", "rb+");
        fp1 = fopen("vd", "rb");

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
                unsigned int tmp = ftell(fp);
                unsigned char tmpbits = bits;
                fread(&buff, 1, 1, fp);
                buff = ((buff >> (8 - (bits + 1))) & 1);
                bits++;
                bits %= 8;
                if (bits != 0)
                {
                        fseek(fp, -1, SEEK_CUR);
                }
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
                if (buff && !dflag)
                {
                        writeMetaStart = tmp;
                        writeMetabits = tmpbits;
                        writeFileStart = vdSize - (totalFSize - atoi(d1.decoded_string));
                        tmpcount--;
                        dflag = 1;
                }
                if (!buff && dflag)
                {
                        cpyMStart = tmp;
                        cpyMBitStart = tmpbits;
                        cpyMetaSize = ceil((double)(d1.tot_bits_read + d2.tot_bits_read) / 8);
                        cpyFileSize = atoi(d1.decoded_string);
                        cpyFileStart = vdSize - (totalFSize - cpyFileSize);
                        ndflag = 1;
                }
                if (dflag && ndflag)
                {
                        unsigned int inBytes = 0;
                        unsigned int outBytes = 0;
                        unsigned char bitsWritten = writeMetabits;
                        unsigned char bitsRead = cpyMBitStart;
                        unsigned char out = 0;
                        unsigned char in = 0;
                        fseek(fp, writeMetaStart, SEEK_SET);
                        if (writeMetabits != 0)
                        {
                                fread(&out, 1, 1, fp);
                        }
                        fseek(fp, cpyMStart, SEEK_SET);
                        if (cpyMBitStart != 0)
                        {
                                fread(&in, 1, 1, fp);
                        }
                        unsigned flag = 0;
                        unsigned flagBits = 0;
                        while (i < cpyMetaSize) // i shows the byte count of input
                        {
                                if (bitsWritten == 8)
                                {
                                        fwrite(&out, 1, 1, fp);
                                        out = 0;
                                        bitsWritten = 0;
                                }
                                if (bitsRead == 8)
                                {

                                        inBytes++;
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
                                if ((bitsWritten == 0) && (bitsRead == 0))
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
                        bitsWritten %= 8;
                        if (bitsWritten != 0)
                        {
                                fwrite(&out, 1, 1, fp);
                        }
                }
        }
}