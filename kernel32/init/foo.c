/*
 * Testing site!
 *
 * Write here any code you want to test with the kernel..
 */
//
#include "ega.h"
#include "foo.h"
#include "stdarg.h"

//
void foo(char *fmt, ...)
{
 va_list ap;
 int d;
 char c, *p, *s;

 //
 va_start(ap, fmt);

 //
 print("beginning foo loop...\n");
 //
 while (*fmt) switch(*fmt++)
 {
  print("*");
 case 's':           /* string */
  s = va_arg(ap, char *);
  print("string "); print(s); print("\n");
  break;
 case 'd':           /* int */
  d = va_arg(ap, int);
  print("int "); p16(d); print("\n");
  break;
 case 'c':           /* char */
  /* need a cast here since va_arg only
     takes fully promoted types */
  c = (char) va_arg(ap, int);
  print("char "); putch(c); print("\n");
  break;
 }
 print("\nleaving foo...\n");
 va_end(ap);
}
 
