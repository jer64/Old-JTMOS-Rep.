#ifndef __INCLUDED_FSFIND_H__
#define __INCLUDED_FSFIND_H__

//
int jtmfs_ffindfile(int device_nr,
                const char *fname,int _block_nr,DIRWALK *dw);
int jtmfs_ffindghostfile(int device_nr,const char *fname,int block_nr,DIRWALK *dw);
int _jtmfs_ffindfirstghostfile(int device_nr,int block_nr,DIRWALK *dw);
int jtmfs_ResolveDirName(int dnr,int db,DIRWALK *dw);

#endif

