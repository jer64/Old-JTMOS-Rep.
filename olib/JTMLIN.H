/* (c)Copyright 1997-1999 by Jari Tapio Tuominen, ABSOLUTLY NO WARRANTY!
   See file COPYING for more information! */
/*
 * Shortcut library v1.0 BETA,
 * makes shortcuts easy to use, try your selves, it's very quick'n'easy.
 *
 * JTM-OS link file format -- If you don't know where some file is located in,
 *                            just create link to it [.LIN].
 * JTM-OS link format(C)1997 by Jari Tuominen, all rights reserved.
 *
 * [Format]:
 *
 * <Link>
 * to: %:\jtm\bin\help.jtx
 * format: Document
 * ...
 *
 */

char *getlink(char *_fname)
{
 char str[255];
 /*
  *
  * Okay, lets get some information about the actual configuration, by now:
  *
  */
 FILE *_fp; /*
             * Own file pointer, to trouble shoot bugs to the recycled bin.
             */
 char _driveletter[1]; /*
                     * Drive letter of drive, where JTM-OS's installed in.
                     * "JTMOS.CFG" == is the configuration file of JTM-OS !
                     *
                     * Part #1: 
                     * Configuration check.
                     *
                     */

 _fp=fopen(jtmos_cfg,"rb");
 _driveletter[0]=fgetc(_fp);
 fclose(_fp);
  /*
   * Now let's check out the link file & good time to add drive letter in the
   * the link showing file name, works like this: Org.:"%:\jtm\docs\great.jtx"
   * ==> "e:\jtm.."
   *
   * Part #2: Link file check
   */

 _fp=fopen(_fname,"rb");
 fscanf(_fp,"%s",&str);
 fclose(_fp);
 str[0]=_driveletter[0];
 return str;
}

void showlinjtx(char *_fname)
{
 char str[255];
 //
 strcpy(str,getlink(_fname));
 cprintf("%s\n\r",str);
 if(showjtx(str)==1)
 {
  exit(10);
 }
 textcolor(7); cprintf("\r\n");
}

