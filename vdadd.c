// argv[1] == vdpath, argv[2] = filepath to copy in vd
#include "encdec.h"

int main(int argc, char *argv[])
{
    FILE *fp, *fp1;
    unsigned int vdSize, givenFileSize, availableSpace, requiredSpace;
    unsigned int totalFSize = 0, metaDataSize = 0;
    if (argc < 3)
    {
        printf("Add file argument missing\n");
        return 0;
    }
    // virtual disk
    if ((fp = fopen(argv[1], "rb+")) == NULL)
    {
        printf("Error opening virtual Disk\n");
        return 0;
    }
    // user file
    if ((fp1 = fopen(argv[2], "rb")) == NULL)
    {
        printf("Error opening specified file\n");
        return 0;
    }
    fseek(fp, 0, SEEK_END);
    vdSize = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    unsigned short count;
    fread(&count, sizeof(count), 1, fp);

    fseek(fp1, 0, SEEK_END);
    givenFileSize = ftell(fp1);
    unsigned int bytes = 0;
    unsigned char bits = 0;
    unsigned char buff;
    for (int i = 0; i < count; i++)
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
        bits = (d1.tot_bits_read - (8 - bits)) % 8;
        if (bits != 0)
        {
            fseek(fp, -1, SEEK_CUR);
        }
        dec_out d2 = decode_file(fp, bits); // reading name of file
        bits = (d2.tot_bits_read - (8 - bits)) % 8;
        metaDataSize += d1.tot_bits_read + 1;
        totalFSize += atoi(d1.decoded_string);
        metaDataSize += d2.tot_bits_read;
    }
    availableSpace = (vdSize - (2 + totalFSize + (int)ceil((double)metaDataSize / 8)));
    totalFSize += givenFileSize;
    unsigned char fileSizeString[10];
    sprintf(fileSizeString, "%d", givenFileSize);

    enc_out e1 = encode(fileSizeString, strlen(fileSizeString));
    enc_out e2 = encode(argv[2], strlen(argv[2]));

    requiredSpace = givenFileSize + strlen(e1.encoded_text) + strlen(e2.encoded_text);
    if (requiredSpace > availableSpace)
    {
        printf("couldn't store the file not enough space\n");
        printf("Required Space: %d, Available: %d\n", requiredSpace, availableSpace);
        return 1;
    }
    if (bits != 0)
    {
        fseek(fp, -1, SEEK_CUR);
    }
    fread(&buff, 1, 1, fp);
    buff = ((buff >> (8 - bits)) << (8 - bits));
    fseek(fp, -1, SEEK_CUR);
    fwrite(&buff, 1, 1, fp);
    bits++;
    bits %= 8;
    if (bits != 0)
    {
        fseek(fp, -1, SEEK_CUR);
    }
    unsigned int tmpfp = ftell(fp);
    cpy_to_fp(fp, e1.encoded_text, strlen(e1.encoded_text), bits);
    bytes = floor((double)(e1.encoded_bits - (8 - bits)) / 8);
    bits = (e1.encoded_bits - (8 - bits)) % 8;
    printf("BYTES %d Bits after writing file size %d  %d\n", bytes, bits, e1.encoded_bits);
    fseek(fp, tmpfp + bytes + 1, SEEK_SET);

    cpy_to_fp(fp, e2.encoded_text, strlen(e2.encoded_text), bits);
    // bytes = floor((double)(e2.encoded_bits - (8 - bits)) / 8);
    // bits = (e2.encoded_bits - (8 - bits)) % 8;
    // printf("BYTES %d Bits after writing file name %d  %d\n", bytes, bits, e2.encoded_bits);
    fseek(fp, ((vdSize)-totalFSize), SEEK_SET);
    fseek(fp1, 0, SEEK_SET);
    // copying file
    unsigned char buffer;
    for (int i = 0; i < givenFileSize; i++)
    {
        fread(&buffer, sizeof(buffer), 1, fp1);
        fwrite(&buffer, sizeof(buffer), 1, fp);
    }
    count++;
    fseek(fp, 0, SEEK_SET);
    fwrite(&count, sizeof(count), 1, fp);
    fclose(fp);
    fclose(fp1);
    return 0;
}