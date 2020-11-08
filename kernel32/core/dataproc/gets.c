//
#include <stdio.h>
#include "gets.h"

// 
char *gets(char *s)
{
    int	i;
    int	key;

    // 
/*    printk("%s/%s/%d: Entered gets prompt.",
    	__FUNCTION__, __FILE__, __LINE__);*/
    strcpy(s, "");

    // 
    for (i = 0; ;)
    {
	// 
	key = getch();

	// ESC
	if (key == 27) {
	    printk("[ESC]");
	}
	// ENTER
	if (key == '\n') {
	    putch(key);
	    break;
	}
	// BACKSPACE
	if (key == '\b') {
	    // 
	    if (i) {
		putch(key);
		i--;
		if (s[i] == 27) {
		    printk("\b\b\b\b");
		}
		s[i] = 0;
	    }
	    continue;
	}
	// OTHER KEYS
	if (i < ((1024*64) - 1)) {
	    if (key != 27)
		putch(key);
	    s[i] = key;
	    s[i + 1] = 0;
	    i++;
	    continue;
	}
    }
    
    //
/*    printk("%s/%s/%d: Success: Now returning back, bringing up the string (content: \"%s\").\n",
    	__FUNCTION__, __FILE__, __LINE__, s);*/

    //
    return s;
}
