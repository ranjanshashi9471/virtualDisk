#include <stdio.h>
int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		printf("disk not given\n");
	}
	FILE *fp = fopen(argv[1], "rb");
	unsigned short count;
	fread(&count, 2, 1, fp);
	printf("File count %s is %d\n", argv[1], count);
	return 0;
}