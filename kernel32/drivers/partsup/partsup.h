#ifndef __INCLUDED_PARTSUP_H__
#define __INCLUDED_PARTSUP_H__

// Something here:
int partition_table_support_init(void);

//
typedef struct
{
	//
	BYTE boot_indicator;
	BYTE beg_chs1; // beginning sector head number
	BYTE beg_chs2; // beginning sector (2 high bits of cylinder #)
	BYTE beg_chs3; // beginning cylinder# (low order bits of cylinder #)
	BYTE system_indicator;
	BYTE end_chs1; // beginning sector head number
	BYTE end_chs2; // beginning sector (2 high bits of cylinder #)
	BYTE end_chs3; // beginning cylinder# (low order bits of cylinder #)
	DWORD begsec;
	DWORD sector_count;
}PARTITION;
//
int partition_table_support_init(void);

#endif
