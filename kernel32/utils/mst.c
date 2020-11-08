//
#include <stdio.h>

//
int main(int argc,char *argv[])
{
	FILE *f,*f2;
	long a,b,c,d,bl;

	//
	printf("mst - Microsoft's [.TXT] to Unix converter(.txt)\n");
	if(argc < 3)
	{
		//
		printf("http://jtmos.8m.com - science.fiction@pp.nic.fi\n");
		printf("Usage: mst [source.txt] [output.txt]\n");
		return 0;
	}

	//
	printf("Now converting ...\n");  
 
	//
	printf("%s to %s\n",argv[1],argv[2]);
	f=fopen(argv[1],"rb"); if(!f){return 1;}
	f2=fopen(argv[2],"wb"); if(!f2){fclose(f); return 2;}
	bl=0;
	while(!0)
	{
		//
		bl++; if(bl==256){ bl=0; printf("="); }
		if(feof(f))break;
		a=fgetc(f); if(feof(f)) { fputc(a,f2); break; }
		b=fgetc(f);
		if(a==0x0a && b==0x0d) { fputc(0x0a,f2); }
		else { fseek(f,-1,SEEK_CUR); fputc(a,f2); }
	}
	printf("\n'%s': %u -- '%s': %u\n",argv[1],ftell(f),argv[2],ftell(f2));
	fclose(f); fclose(f2);

	//
	return 0;
}
