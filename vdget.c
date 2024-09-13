// argv[1] == vdpath, argv[2] == fileno, argv[3] == destn file name

#include "encdec.h"

int main(int argc, char *argv[])
{
	if (argc < 4)
	{
		printf("Proper Argument not given\n");
		return 0;
	}
	unsigned short fileno = atoi(argv[2]);
	unsigned short count;
	unsigned int vdsize;
	unsigned int skipfilebytes = 0;
	FILE *fp, *fp1;
	if ((fp1 = fopen(argv[3], "ab")) == NULL)
	{
		printf("Error opening %s\n", argv[3]);
		return 0;
	}
	if ((fp = fopen(argv[1], "rb+")) == NULL)
	{
		printf("Error opening %s\n", argv[1]);
		return 0;
	}
	fread(&count, 2, 1, fp);
	if (fileno > count)
	{
		printf("fileno is greater than files on disk\n");
		return 0;
	}
	dec_out d1;
	unsigned char bits = 0;
	for (int i = 0; i < fileno; i++)
	{
		if (bits != 0)
		{
			fseek(fp, -1, SEEK_CUR);
		}
		d1 = decode_file(fp, bits);
		bits = d1.lst_byte;
		skipfilebytes += atoi(d1.decoded_string);
	}
	fseek(fp, 0, SEEK_END);
	vdsize = ftell(fp);
	fseek(fp, vdsize - skipfilebytes, SEEK_SET);
	unsigned char buff;
	for (int i = 0; i < atoi(d1.decoded_string); i++)
	{
		fread(&buff, sizeof(buff), 1, fp);
		fwrite(&buff, sizeof(buff), 1, fp1);
	}
	fclose(fp1);
	fclose(fp);
	return 0;
}
