/* (c)Copyright 1997-1999 by Jari Tapio Tuominen, ABSOLUTLY NO WARRANTY!
   See file COPYING for more information! */
#define jtmos_multitask TRUE
void jtm_multitasker(void)
{
 if(fp!=NULL)
 {
  fputc(0x00,fp);
 }
}
