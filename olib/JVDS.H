/* (c)Copyright 1997-1999 by Jari Tapio Tuominen, ABSOLUTLY NO WARRANTY!
   See file COPYING for more information! */
/*
 * Jari's Virtual Directory/filing System, JVDS 1.00,
 * (C)1998 by Jari Tapio Tuominen, all rights reserved.
 *
 * Some facts :
 *
 * Filename can be max. 512 marks long, which is quite a lot marks.
 *
 *
 *
 *
 */


// jfopen
// ------
//
// Reads/Writes file with long filenames and additional format specifier.
//
FILE *jfopen(char *fname,char *waytohandle)
{
 // Some local variables
 unsigned long ad,ad2;
 FILE *fptemp,*f,*f2;
 char *dosfilename,*jfilename,*str,*str2,*str3,*str4;
 unsigned long nextval; // Next value to use for DOS name creation

 // -Allocate memory for filename
 dosfilename=malloc(512);
 jfilename=malloc(512);
 str=malloc(512);
 str2=malloc(512);
 str3=malloc(512);
 str4=malloc(512);

 write("Testing\n");

 // Load catalog information file.
 f=fopen("jcatalog.cin","rb");
 if(f==NULL)
 {
  nextval=0;
  goto nocatalogfound;
 }
 fscanf(f,"%lu",&nextval);
 fclose(f);
 //
 for(ad=1; ad<nextval; ad++)
 {
  sprintf(str, "%1.8x.fil",ad);
  sprintf(str2,"%1.8x.fin",ad);
  f=fopen(str2,"rb");
  if(f==NULL)break;
  fscanf(f,"%s",&str3);
  fclose(f);
  sprintf(str4,"Compare %s,%s\n",str3,fname);
  write(str4);
  if(!strcmp(str3,fname))
  {
   return NULL; // Return NULL if file already exist.
  }
 }
nocatalogfound:
 //

 // Increase nextval
 nextval++;

 // Create/Replace CATALOG information file with new value.
 f=fopen("jcatalog.cin","wb");
 fprintf(f,"%lu",nextval);
 fclose(f);

 // Create DOS filename
 sprintf(dosfilename, "%1.8x.fil",nextval);
 sprintf(jfilename,   "%1.8x.fin",nextval);

 // Create file information file [.FIL]
 f=fopen(jfilename,"wb");
 fprintf(f,"%s",fname);
 fclose(f);

 // Finally open/create the file with DOS name.
 fptemp=fopen(dosfilename,waytohandle);

 free(dosfilename);
 free(jfilename);
 free(str);
 free(str2);
 free(str3);
 free(str4);
 return fptemp;
}
