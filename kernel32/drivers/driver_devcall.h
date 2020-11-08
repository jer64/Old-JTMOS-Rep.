//
#ifndef __INCLUDED_DEVAPI_DEVCALL_H__
#define __INCLUDED_DEVAPI_DEVCALL_H__

//
int device_call(long device_nr,
                long n_function,
                long par1,long par2,long par3,long par4,
                void *p1,void *p2);

#endif
