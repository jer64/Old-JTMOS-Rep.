/* (c)Copyright 1997-1999 by Jari Tapio Tuominen, ABSOLUTLY NO WARRANTY!
   See file COPYING for more information! */
/*
 *    JTMOS - Application configuration management system. (ACMS)
 *
 *    (C)1998 by Jari Tapio Tuominen, all rights reserved.
 */
#define JTMACFG_INCLUDED

int jtmconfig_loadconfig(void)
{
 FILE *f;
 unsigned long int ad,ad2;
 //
 f=jtm_fopen(appconfig.fname_cfg,"rb");
 if(f==NULL)return 1;
 ad=0;
 do
 {
  if(feof(f))break;
  fscanf(f,"%u",&appconfig.data[ad]);
  ad++;
 }while(0==0);
 jtm_fclose(f);
 return 0;
}

int jtmconfig_saveconfig(void)
{
 // Some locals
 FILE *f;
 unsigned long int ad,ad2;

 // If configuration file doesn't exist then
 // initialize current configuration in memory,
 // which is anyway probaly just a couple random numbers.

 //
 f=jtm_fopen(appconfig.fname_cfg,"wb");
 if(f==NULL)return 1;
 ad=0;
 do
 {
  fprintf(f,"%u ",appconfig.data[ad]);
  ad++;
  if(ad>=255)break;
 }while(0==0);
 jtm_fclose(f);
 return 0;
}

