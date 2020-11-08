// bin2c.c 1.2 (C) 2002-2006 by Jari Tuominen.
#include <stdio.h>

//
int main(int argc,char **argv)
{
	unsigned char bits[]={128,64,32,16,8,4,2,1};
	unsigned int i,i2,i3,i4,i5,i6,i7,i8,d[8];
	FILE *f;
	char ch;
	char array_name[512];

	//
	if(argc<2)
	{
		//
		printf("bin2c 1.2 by Jari Tuominen(jari@vunet.org).\n");
		printf("Usage: bin2c [source] [array name(optional)]\n");
		printf("Description: converts binary files to C arrays\n");
		printf("that can be used in C-language programs.\n");
		printf("Outputs to stdout.\n");
		return 0;
	}

	//
	if(argc>2)
	 	strcpy(array_name,argv[2]);
		else
		strcpy(array_name,"bin1");

	//
	f=fopen(argv[1],"rb");
	if(f==NULL)
	{
		fprintf(stderr,"bin2c: cannot find the source file '%s'\n",argv[1]);
		return 0;
	}

	//
	fseek(f,0,SEEK_END); i2=ftell(f);
	fseek(f,0,SEEK_SET);


	//
	fprintf(stdout, "// %s - converted from binary to array\n", argv[1]);
	fprintf(stdout, "int %s_length=%d;\n", array_name, i2);
	fprintf(stdout, "unsigned char %s[]={\n", array_name);

	//
	for(i=0; i<i2; i+=16)
	{
		//
		if(feof(f))break;
		fprintf(stdout,"\t");
		if( (i+16)>=i2 )
			ch=' ';
		else
			ch=',';
		for(i3=i; i3<i+16; i3++)
		{
			if(i3==(i+16-1))
			{
				fprintf(stdout,"%d%c",fgetc(f),ch);
			}
			else
			{
				fprintf(stdout,"%d,",fgetc(f));
			}
		}
		fprintf(stdout," // %1.8x\n",i);
	}

	//
	fprintf(stdout,"};\n//end of array\n");

	//
	fclose(f); 

	//
	return 0;
}


