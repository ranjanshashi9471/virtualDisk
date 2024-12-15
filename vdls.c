#include "encdec.h"

int main(int argc, char *argv[])
{
        if (argc < 2)
        {
                printf("Error: Virtual disk file missing.\n");
                return 1;
        }

        FILE *fp;
        unsigned int vdSize;
        unsigned int totalFSize = 0, metaDataSize = 0;

        // Open virtual disk
        if ((fp = fopen(argv[1], "rb+")) == NULL)
        {
                perror("Error opening virtual disk");
                return 1;
        }

        // Read file count
        fseek(fp, 0, SEEK_SET);
        unsigned short count;
        if (fread(&count, sizeof(count), 1, fp) != 1)
        {
                printf("Error reading file count from virtual disk.\n");
                fclose(fp);
                return 1;
        }

        unsigned char bits = 0, bytes = 0, buff = 0;

        // Display file metadata
        for (int i = 0; i < count; i++)
        {
                if (bits != 0)
                {
                        fseek(fp, -1, SEEK_CUR);
                }

                if (fread(&buff, 1, 1, fp) != 1)
                {
                        printf("Error reading file %d metadata.\n", i + 1);
                        fclose(fp);
                        return 1;
                }

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

                metaDataSize += d1.tot_bits_read + 1;
                metaDataSize += d2.tot_bits_read;
                totalFSize += atoi(d1.decoded_string);

                if (!buff)
                {
                        printf("%d\t %s\t %s\n", i + 1, d1.decoded_string, d2.decoded_string);
                }
        }

        // Calculate available space
        fseek(fp, 0, SEEK_END);
        vdSize = ftell(fp);
        printf("Virtual Disk Size: %ld bytes\n", vdSize);
        printf("Metadata Size: %d bytes\n", (int)ceil((double)metaDataSize / 8));
        printf("Available space: %d bytes\n", (vdSize - (2 + totalFSize + (int)ceil((double)metaDataSize / 8))));

        fclose(fp);
        return 0;
}
