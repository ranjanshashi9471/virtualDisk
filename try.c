#include<stdlib.h>
#include<stdio.h>

unsigned char* int_to_str(unsigned int a){
	unsigned int b = a;
	unsigned int digits = 0;

	while(b>0){
		b = b/10;
		digits++;
	}
	printf("digits::%d\n", digits);
	b = a;
	unsigned char *out = malloc(digits+1);
	for(int i = digits-1; i >= 0; i--){
		unsigned int r = b%10;
		out[i] = r;
		printf("out[%d]::%d\n", i, out[i]);
		b = b/10;
	}
	out[digits] = '\0';
	for(int i = 0; i<digits; i++){
		printf("out[%d]::%c\n", i, out[i]);
	}
	printf("done int to str %s\n", out);

	return out;
}

int main(){
	unsigned int a = 12345678;
	printf("string is %s\n",int_to_str(a));
	return 0;
}
