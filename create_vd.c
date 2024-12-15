#include "encdec.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <size> <virtual_disk_name>\n", argv[0]);
        return 1;
    }

    int size = atoi(argv[1]);
    if (size < 2)
    {
        fprintf(stderr, "Error: Disk size must be at least 2 bytes.\n");
        return 1;
    }

    FILE *fp = fopen(argv[2], "wb+");
    if (fp == NULL)
    {
        perror("Failed to create virtual disk");
        return 1;
    }
    printf("%s Created\n", argv[2]);

    // Allocate and write placeholder data
    unsigned char *buffer = (unsigned char *)calloc(1, size);
    if (buffer == NULL)
    {
        perror("Memory allocation failed");
        fclose(fp);
        return 1;
    }
    fwrite(buffer, 1, size, fp);
    free(buffer);

    // Write metadata
    unsigned short count = 0;
    fseek(fp, 0, SEEK_SET);
    fwrite(&count, sizeof(count), 1, fp);

    fclose(fp);
    return 0;
}
