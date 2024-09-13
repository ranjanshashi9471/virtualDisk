// argv[1] == size of virtual disk, argv[2] == name of virtual drive
#include "encdec.h"

int main(int argc, char *argv[])
{
    FILE *fp;
    unsigned short count = 0;
    fp = fopen(argv[2], "wb+");
    if (fp == NULL)
    {
        perror("Failed to create vd");
        return 0;
    }
    printf("%s Created\n", argv[2]);
    for (int i = 0; i < atoi(argv[1]); i++)
    {
        fwrite("0", 1, 1, fp);
    }
    fseek(fp, 0, SEEK_SET);
    fwrite(&count, sizeof(count), 1, fp);
    fclose(fp);
    return 0;
}
