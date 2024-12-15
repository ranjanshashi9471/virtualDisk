#include <stdio.h>

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		printf("Error: Virtual disk not specified.\n");
		return 1; // Exit with an error code
	}

	FILE *fp = fopen(argv[1], "rb");
	if (fp == NULL)
	{
		perror("Error opening virtual disk");
		return 1; // Exit with an error code
	}

	unsigned short count = 0;
	if (fread(&count, sizeof(count), 1, fp) != 1)
	{
		printf("Error: Failed to read file count from virtual disk.\n");
		fclose(fp);
		return 1; // Exit with an error code
	}

	printf("File count in virtual disk '%s' is %d\n", argv[1], count);

	fclose(fp); // Close the file to release resources
	return 0;	// Successful execution
}
