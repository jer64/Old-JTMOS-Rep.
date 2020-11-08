#ifndef __INCLUDED_HDCACHEACCESS_H__
#define __INCLUDED_HDCACHEACCESS_H__

//
HCE *hdLoadNewTrack(int C, int H);
HCE *hdFindTrackFromCache(int C, int H);
BYTE *hdCacheAccess(int C, int H);
int hdCacheRW(int c, int h, int s,  int rw,
                BYTE *buf, int l_buf);
void hdCacheShowInfo(void);
void hdCacheCheckBuffers(void);
void hdCacheCheckBuffers1(const char *func, int line);

#endif




