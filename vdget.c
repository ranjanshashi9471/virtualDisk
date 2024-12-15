#include "encdec.h"

int main(int argc, char *argv[])
{
	if (argc < 4)
	{
		printf("Error: Proper arguments not given.\nUsage: ./vdget <virtual_disk> <file_number> <destination_file>\n");
		return 1;
	}

	unsigned short fileno = atoi(argv[2]);
	unsigned short count;
	unsigned int vdsize, skipfilebytes = 0;
	FILE *fp, *fp1;

	// Open virtual disk
	if ((fp = fopen(argv[1], "rb")) == NULL)
	{
		perror("Error opening virtual disk");
		return 1;
	}

	// Open destination file in write mode
	if ((fp1 = fopen(argv[3], "wb")) == NULL)
	{
		perror("Error opening destination file");
		fclose(fp);
		return 1;
	}

	// Read file count
	if (fread(&count, sizeof(count), 1, fp) != 1)
	{
		printf("Error reading file count from virtual disk.\n");
		fclose(fp);
		fclose(fp1);
		return 1;
	}

	// Validate file number
	if (fileno > count || fileno < 1)
	{
		printf("Error: Invalid file number. File does not exist.\n");
		fclose(fp);
		fclose(fp1);
		return 1;
	}

	dec_out d1, d2;
	unsigned char bits = 0, buff = 0;

	// Navigate to the target file metadata
	for (int i = 0; i < fileno; i++)
	{
		if (bits != 0)
			fseek(fp, -1, SEEK_CUR);

		if (fread(&buff, 1, 1, fp) != 1)
		{
			printf("Error reading metadata for file %d.\n", i + 1);
			fclose(fp);
			fclose(fp1);
			return 1;
		}

		bits++;
		bits %= 8;

		if (bits != 0)
			fseek(fp, -1, SEEK_CUR);

		d1 = decode_file(fp, bits); // Read file size
		bits = (d1.tot_bits_read - (8 - bits)) % 8;

		if (bits != 0)
			fseek(fp, -1, SEEK_CUR);

		d2 = decode_file(fp, bits); // Read file name
		bits = (d2.tot_bits_read - (8 - bits)) % 8;

		skipfilebytes += atoi(d1.decoded_string);
	}

	// Locate the start of the file content
	fseek(fp, 0, SEEK_END);
	vdsize = ftell(fp);
	fseek(fp, vdsize - skipfilebytes, SEEK_SET);

	// Extract the file
	unsigned int fileSize = atoi(d1.decoded_string);
	printf("Extracting file: %s (Size: %u bytes)\n", d2.decoded_string, fileSize);

	for (unsigned int i = 0; i < fileSize; i++)
	{
		if (fread(&buff, 1, 1, fp) != 1 || fwrite(&buff, 1, 1, fp1) != 1)
		{
			printf("Error extracting file. Operation aborted.\n");
			fclose(fp);
			fclose(fp1);
			return 1;
		}
	}

	printf("File extracted successfully to %s\n", argv[3]);
	fclose(fp);
	fclose(fp1);
	return 0;
}
