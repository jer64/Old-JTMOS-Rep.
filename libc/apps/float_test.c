// float_test.c
#include <stdio.h>

int main(int argc, char **argv)
{
	char str[256];
	char name[256];
	double f1,f2,f3,f4,f5,f6,f7,f8,f9,f10;
	double predefined_float=1234567.6553699999;

	printf("sscanf_test.c - sscanf LIBC function test program\n");
	printf("Type 10 different decimal numbers here\n");
	printf("(or just hit enter to test printf quickly\n");
	printf("numbers: ");
	gets(str);

	if(!strlen(str))
	{
		printf("predefined_float = %lf\n",
			predefined_float);
		printf("predefined_float = %lf\n",
			predefined_float*10);
		printf("predefined_float = %lf\n",
			predefined_float*100);

		goto past;
	}

	f1=0;
	f2=0;
	f3=0;
	f4=0;
	f5=0;
	f6=0;
	f7=0;
	f8=0;
	f9=0;
	f10=0;
	sscanf(str, "%lf %lf %lf %lf %lf  %lf %lf %lf %lf %lf",
		&f1, &f2, &f3, &f4, &f5,
		&f6, &f7, &f8, &f9, &f10);
	printf("f1=%f\n", f1);
	printf("f2=%f\n", f2);
	printf("f3=%f\n", f3);
	printf("f4=%f\n", f4);
	printf("f5=%f\n", f5);
	printf("f6=%f\n", f6);
	printf("f7=%f\n", f7);
	printf("f8=%f\n", f8);
	printf("f9=%f\n", f9);
	printf("f10=%f\n", f10);
	printf("\n");
past:
	return 0;
}
