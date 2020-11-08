;
; JTMOS Bootsector Header
; Based on FAT12 standard bootsector.
;
Copyright:		db "JTMOS"
OsVersion:		db "PA1"	;;;; Pre-Alpha 1
BytesPerSector:		dw 1
SectorsPerCluster:	db 8
ReservedSectors		db 1
n_fats			db 1
n_direntries		dw 512
n_totalsectors		dw 2880
MediaByte:		db 0xF8
n_fatsectors		dw 8
sectorspertrack:	dw 17
n_heads			dw 4
n_hiddensectors		dd 1
n_bigtotalsectors	dd 0
bootdrive		db 0x80
currenthead		db 0
extrabootsig		db 41
serialnumber		dd 0xFCE2E37B
volumestr		db "SYSTEM     "
fatid			db "JTMFS   "

