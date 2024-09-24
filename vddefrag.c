#include "encdec.h"

int main()
{
        FILE *fp;
        unsigned int vdSize;
        unsigned int totalFSize = 0, metaDataSize = 0;
        unsigned int writeMetaStart, writeMetabits, writeFileStart, cpyMStart, cpyMBitStart, cpyMetaSize, cpyFileStart, cpyFileSize;
        fp = fopen("vd", "rb+");
        unsigned short count;
        unsigned short tmpcount = count;
        fread(&count, 2, 1, fp);
        unsigned char bits = 0;
        unsigned char bytes = 0;
        unsigned char buff;
        for (int i = 0; i < count; i++)
        {
                if (bits != 0)
                {
                        fseek(fp, -1, SEEK_CUR);
                }
                writeMetaStart = ftell(fp);
                writeMetabits = bits;
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
                        writeFileStart = vdSize - (totalFSize - atoi(d1.decoded_string));
                        if (bits != 0)
                        {
                                fseek(fp, -1, SEEK_CUR);
                        }
                        cpyMStart = ftell(fp);
                        unsigned int tmptotFileSize = totalFSize;
                        for (int j = i + 1; j < count; j++)
                        {
                                if (bits != 0)
                                {
                                        fseek(fp, -1, SEEK_CUR);
                                }
                                cpyMStart = ftell(fp);
                                cpyMBitStart = bits;
                                bits++;
                                if (bits != 0)
                                {
                                        fseek(fp, -1, SEEK_CUR);
                                }
                                d1 = decode_file(fp, bits);
                                bits = (d1.tot_bits_read - (8 - bits)) % 8;
                                if (bits != 0)
                                {
                                        fseek(fp, -1, SEEK_CUR);
                                }
                                d2 = decode_file(fp, bits); // reading name of file
                                bits = (d2.tot_bits_read - (8 - bits)) % 8;
                                // metaDataSize += d1.tot_bits_read + 1;
                                // metaDataSize += d2.tot_bits_read;
                                totalFSize += atoi(d1.decoded_string);
                                cpyMetaSize = (d1.tot_bits_read + d2.tot_bits_read + 1);
                                cpyFileSize = atoi(d1.decoded_string);
                                cpyFileStart = vdSize - (totalFSize - cpyFileSize);

                                // generating string from file to shift

                                fseek(fp, cpyMStart, SEEK_SET);
                                unsigned int outBytes = 0;
                                unsigned char bitsWritten = 0;
                                unsigned char bitsRead = cpyMBitStart;
                                unsigned malloclimit = ceil((double)(cpyMetaSize) / 8);
                                unsigned char *outArr = malloc(malloclimit);
                                unsigned char in = 0;
                                if (cpyMBitStart != 0)
                                {
                                        fread(&in, 1, 1, fp);
                                }
                                unsigned flag = 0;
                                unsigned flagBits = 0;

                                while (outBytes < malloclimit) // i shows the byte count of input
                                {
                                        if (bitsWritten == 8)
                                        {
                                                outBytes++;
                                                bitsWritten = 0;
                                        }
                                        if (bitsRead == 8)
                                        {
                                                fread(&in, 1, 1, fp);
                                                bitsRead = 0;
                                        }
                                        if (flag)
                                        {
                                                flag = 0;
                                                outArr[outBytes] |= (in >> flagBits);
                                                outBytes++;
                                                bitsRead = 8 - flagBits;
                                                flagBits = 0;
                                                bitsWritten = 0;
                                        }
                                        if ((bitsWritten == 0) && (bitsRead == 0))
                                        {
                                                outArr[outBytes] = in;
                                                bitsRead = 8;
                                                bitsWritten = 8;
                                        }
                                        else
                                        {
                                                outArr[outBytes] = (outArr[outBytes] | (in << bitsRead));
                                                flag = 1;
                                                flagBits = 8 - bitsRead;
                                                bitsWritten = flagBits;
                                                bitsRead = 8;
                                        }
                                }
                                fseek(fp, writeMetaStart, SEEK_SET);
                                cpy_to_fp(fp, outArr, strlen(outArr), writeMetabits);
                                cpyMStart = cpyMStart + 1 + floor((double)(cpyMetaSize - (8 - cpyMBitStart)) / 8);
                                cpyMBitStart = (cpyMetaSize - (8 - cpyMBitStart)) % 8;
                                writeMetaStart = writeMetaStart + 1 + floor((double)(cpyMetaSize - (8 - writeMetaStart)) / 8);
                                writeMetabits = (cpyMetaSize - (8 - writeMetabits)) % 8;

                                // shifting the file now
                                int k;
                                for (k = 0; k < cpyFileSize; k++)
                                {
                                        fseek(fp, cpyFileStart - k, SEEK_SET);
                                        fread(&in, 1, 1, fp);
                                        fseek(fp, writeFileStart - k, SEEK_SET);
                                        fwrite(&in, 1, 1, fp);
                                }
                                writeFileStart = writeFileStart - k;
                                cpyFileStart = cpyFileStart - k;
                        }
                }
        }
        count = tmpcount;
        fseek(fp, 0, SEEK_SET);
        fwrite(&count, 2, 1, fp);
}