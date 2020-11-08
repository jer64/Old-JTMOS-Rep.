//
#ifndef __INCLUDED_DPI_H__
#define __INCLUDED_DPI_H__

//
int registerNewDevice(const char *name,
        void *call,int cs,
        int pid,int type,
        int alias);
int getdevicename(int dnr, char *name);
int getdevicenrbyname(const char *name);
int getcurdev(void);
int DeviceCall(int dnr,
        int n_function,
        int v1,int v2,int v3,int v4,
        void *p1,void *p2);

#endif




