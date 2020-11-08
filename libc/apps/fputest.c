// fputest.c - tests math coprocessor.
#include <stdio.h>

//
int main(int argc, char **argv)
{
	float number1,number2;

	//
	number1 = 654.7623487;
	number2 = 123.7534737;
	printf("Math coprocessor test program.\n");
	printf("%f x %f = %f\n",
		number1, number2, number1*number2);

	//
	return 0;
}

//




