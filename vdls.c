#include "encdec.h"

int main(int argc, char *argv[])
{
        FILE *fp;
        unsigned int vdSize;
        unsigned int totalFSize = 0, metaDataSize = 0;
        if (argc < 2)
        {
                printf("vd file missing\n");
                return 0;
        }
        // virtual disk
        if ((fp = fopen(argv[1], "rb+")) == NULL)
        {
                printf("Error opening virtual Disk\n");
                return 0;
        }
        fseek(fp, 0, SEEK_SET);
        unsigned short count;
        fread(&count, sizeof(count), 1, fp);
        unsigned char bits = 0;
        unsigned char bytes = 0;
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
                dec_out d1 = decode_file(fp, bits); // reading length of file
                bits = (d1.tot_bits_read - (8 - bits)) % 8;
                if (bits != 0)
                {
                        fseek(fp, -1, SEEK_CUR);
                }
                dec_out d2 = decode_file(fp, bits); // reading name of file
                bits = (d2.tot_bits_read - (8 - bits)) % 8;
                metaDataSize += d1.tot_bits_read;
                metaDataSize += d2.tot_bits_read;
                totalFSize += atoi(d1.decoded_string);
                if (!buff)
                {
                        printf("%d\t %s\t %s\n", i + 1, d1.decoded_string, d2.decoded_string);
                }
        }
        fseek(fp, 0, SEEK_END);
        vdSize = ftell(fp);
        printf("vd Size is %ld\n", ftell(fp));
        printf("metadatasize :: %d\n", (int)ceil((double)metaDataSize / 8));
        printf("Available space is %d\n", (vdSize - (2 + totalFSize + (int)ceil((double)metaDataSize / 8))));
        fclose(fp);
        return 0;
}