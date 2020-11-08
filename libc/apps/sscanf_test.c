// sscanf_test.c
#include <stdio.h>

//
int main(int argc, char **argv)
{
	char str[256];
	char name[256];
	int n1,n2,n3,n4,n5,n6,n7,n8,n9,n10;

	//
	printf("sscanf_test.c - sscanf LIBC function test program\n");
	printf("Type 10 different numbers here: ");
	gets(str);
	sscanf(str, "%d %d %d %d %d  %d %d %d %d %d",
		&n1, &n2, &n3, &n4, &n5,
		&n6, &n7, &n8, &n9, &n10);
	printf("n1=%d ", n1);
	printf("n2=%d ", n2);
	printf("n3=%d ", n3);
	printf("n4=%d ", n4);
	printf("n5=%d ", n5);
	printf("n6=%d ", n6);
	printf("n7=%d ", n7);
	printf("n8=%d ", n8);
	printf("n9=%d ", n9);
	printf("n10=%d ", n10);
	printf("\n");

	//
	printf("What is your name?: ");
	gets(str);
	sscanf(str, "%s", &name);
	printf("Nice to meet you, %s.\n", name);

	//
	return 0;
}
