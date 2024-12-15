#include "encdec.h"

int main(int argc, char *argv[])
{
        if (argc < 3)
        {
                printf("Error: Arguments missing. Usage: ./vddelete <virtual_disk> <file_number>\n");
                return 1;
        }

        FILE *fp;
        unsigned short fileno = atoi(argv[2]);
        if ((fp = fopen(argv[1], "rb+")) == NULL)
        {
                perror("Error opening virtual disk");
                return 1;
        }

        unsigned short count;
        if (fread(&count, sizeof(count), 1, fp) != 1)
        {
                printf("Error reading file count from virtual disk.\n");
                fclose(fp);
                return 1;
        }

        if (fileno > count || fileno < 1)
        {
                printf("Error: Invalid file number. File doesn't exist.\n");
                fclose(fp);
                return 1;
        }

        unsigned char bits = 0;
        unsigned char buff;
        for (int i = 0; i < fileno - 1; i++)
        {
                if (bits != 0)
                        fseek(fp, -1, SEEK_CUR);

                if (fread(&buff, 1, 1, fp) != 1)
                {
                        printf("Error reading metadata for file %d.\n", i + 1);
                        fclose(fp);
                        return 1;
                }

                bits++;
                bits %= 8;

                if (bits != 0)
                        fseek(fp, -1, SEEK_CUR);

                dec_out d1 = decode_file(fp, bits); // Reading file size
                bits = (d1.tot_bits_read - (8 - bits)) % 8;

                if (bits != 0)
                        fseek(fp, -1, SEEK_CUR);

                dec_out d2 = decode_file(fp, bits); // Reading file name
                bits = (d2.tot_bits_read - (8 - bits)) % 8;
        }

        if (bits != 0)
                fseek(fp, -1, SEEK_CUR);

        if (fread(&buff, 1, 1, fp) != 1)
        {
                printf("Error reading metadata byte for file %d.\n", fileno);
                fclose(fp);
                return 1;
        }

        // Mark the file as deleted
        buff = buff | (((buff >> (8 - (bits + 1))) | 1) << (8 - (bits + 1)));
        fseek(fp, -1, SEEK_CUR);

        if (fwrite(&buff, 1, 1, fp) != 1)
        {
                printf("Error marking file %d as deleted.\n", fileno);
                fclose(fp);
                return 1;
        }

        printf("File %d marked as deleted successfully.\n", fileno);
        fclose(fp);
        return 0;
}
