/* (c)Copyright 1997-1999 by Jari Tapio Tuominen, ABSOLUTLY NO WARRANTY!
   See file COPYING for more information! */
/*
 *
 *      JTMOS World Wide Web support library
 *
 *
 */

jtmweb_beginhtml(char *title)
{
 printf("<html>\n\n");
 printf("<head>\n");
 printf("<meta http-equiv=\"Content-Type\"\n");
 printf("content=\"text/html; charset=iso-8859-1\">\n");
 printf("<meta name=\"GENERATOR\" content=\"JTMOS & %s\">\n",jtm_aname);
 printf("<title>%s</title>\n",title);
 printf("</head>\n");
}

jtmweb_endhtml()
{
 printf("</body>\n");
 printf("</html>\n");
}

jtmweb_setcolor(unsigned long int color)
{
 printf("<font color=\"#%1.6x\">\n",color);
}

jtmweb_setbcolor(unsigned long int color)
{
 printf("<body bgcolor=\"#%1.6x\">\n",color);
}

jtmweb_strongon()
{
 printf("<strong>");
}

jtmweb_strongoff()
{
 printf("</strong>");
}

jtmweb_writecen(char *text)
{
 printf("<p align=\"center\">%s</p>\n",text);
}

jtmweb_tableon(long bsize,long bgcolor)
{
 printf("<table border=\"%u\" bgcolor=\"#%1.6x\">\n",bsize,bgcolor);
}

jtmweb_tableoff()
{
 printf("</table>\n");
}
