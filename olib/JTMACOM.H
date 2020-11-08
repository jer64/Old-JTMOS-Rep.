/* (c)Copyright 1997-1999 by Jari Tapio Tuominen, ABSOLUTLY NO WARRANTY!
   See file COPYING for more information! */
//
//      JTMOS application/system communicating bridge
//      for the Operating System.
//      See also JTMOS A/S com.bridge for the applications.
//

/*


        ACOF description :


        (offset)        (description)
        0x0000          Test byte. Set this byte to 0 and it'll
                        become a zero if JTMOS exists.
        0x0001          Chat byte. Set this byte to 0 when you
                        want to chat with JTMOS,
                        this byte is allways used when waiting
                        answer from JTMOS or if you are
                        giving commands to JTMOS.

        0x0002-0x0040   Data area (0x3e bytes. Usually before
                        you want to chat with JTMOS you will probaly
                        tell JTMOS your purpose by storing
                        the data with your cmd data and then
                        you should set the chat byte and wait
                        for answer.

        Data area :

        0x0002+0x0003   Specifies the 16bit command number.
                        Each number has own meaning.

                        Commands available :

                        0x0000  JTMOS version
                        0x0001  Amount of free physical memory left
                        0x0002  Amount of free virtual memory left
                        0x0003  jtm_vmode
                        0x0004  jtmscr.width
                        0x0005  jtmscr.height
                        0x0006  call: jtm_setvesa( ACOF+0x04 );
                        0x0007  call: write( ACOF+0x04 );
                        0x0008  ... and so on ...

        0x0004,5,6,7    Specifies the 32bit argument #1
        0x0008,9,a,b    Specifies the 32bit argument #2
        0x000c,d,e,f    Specifies the 32bit argument #3
*/

APICOM_RetrieveMsgs()
{
 //
 char *buf,chat=0;

 //
 chat=0;
 buf=malloc(0x40);
 dosmemget(ACOF,0x40,buf);

 //
 if(!buf[0])
 {
  chat=1;
  buf[0]=0xff;
 }

 //
 if(chat)dosmemput(buf,0x40,ACOF);

 //
 free(buf);
}

APICOM_TurnOff()
{
 //
}

APICOM_Initialize()
{
 //
 char *buf,chat=0;
 long ad;

 //
 chat=0;
 buf=malloc(0x40);
 for(ad=0; ad<0x40; ad++)buf[ad]=0;
 dosmemput(buf,0x40,ACOF);
 free(buf);
}

APICOM_TurnOn()
{
 //
 APICOM_Initialize();
 TM_AddProcess(APICOM_RetrieveMsgs);
}
