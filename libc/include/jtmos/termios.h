#ifndef __INCLUDED_JTMLIBC_TERMIOS_H__
#define __INCLUDED_JTMLIBC_TERMIOS_H__

#define tcflag_t int
#define cc_t int
#define NCCS 10

typedef struct termios
{
	tcflag_t c_iflag;
	tcflag_t c_oflag;
	tcflag_t c_cflag;
	tcflag_t c_lflag;
	cc_t c_cc[NCCS];
};

int tcgetattr(int fd, struct termios *termios_p);

#endif


