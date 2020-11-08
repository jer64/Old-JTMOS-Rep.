//
#ifndef __INCLUDED_LAUNCHAPP_H__
#define __INCLUDED_LAUNCHAPP_H__

//
#define RTBASE		0x80000000
#define RTCODE		0x80004000

// To change this setting, you must also change following this:
// - LaunchApp.h(this)
// - launchapp.asm
// - LIBC linker configuration script
//
#define APPLICATION_START_AD	0x10000

// 64,128,256,512,1024,... K to spend on application.
// 256 is a moderate one.
// Note: There will be a customization for this in future.
// So that it can be defined in the exutable file's header
// part, that how much it allocates memory for the process.
#define DEF_APP_SPACE_K         4096

// Return value: process PID
int LaunchApp(BYTE *abin, int length);
int LaunchAppEx(BYTE *abin, int length, int banks,  int dnr, int db);

//
DWORD launchapp_cleartogo;

//
#endif

//

