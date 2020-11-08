/* (c)Copyright 1997-1999 by Jari Tapio Tuominen, ABSOLUTLY NO WARRANTY!
   See file COPYING for more information! 


            V I R T U A L  E N V I R O N M E N T  L I B R A R Y

            (C)1998 by Jari Tapio Tuominen, all rights reserved.

            This library simulates SFS file system.



*/

void jtmfs_newsfs(char *fn_image,long length)
{
 FILE *f;
 //
 f=fopen(fn_image,"wb");
 fseek(f,length,SEEK_SET);
 fclose(f);
}




