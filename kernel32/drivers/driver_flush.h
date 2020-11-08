#ifndef __INCLUDED_FLUSH_H__
#define __INCLUDED_FLUSH_H__

void driver_TriggerCacheFlush(void);
void _FlushCache(void);
void driver_FlushDrive(int dnr);
void driver_ResetCache(void);
void _ClearCache(void);
void driver_CleanUpCache(void);
void CacheFlushBlocks(int WHICH_DNR);
void driver_FlushWriteCache(void);
int driver_GetCacheFreeBlocks(void);
void driver_CacheUpkeep(void);
void FlushAll(void);
void DiskChangeDNR(int WHICH_DNR);
void driver_TotallyClearCache(void);

#endif
