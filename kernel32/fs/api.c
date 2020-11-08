// ===========================================================
// api.c - Kernel wrappers for LIBC fread, fwrite, etc.
// (C) 2002-2005 by Jari Tuominen.
// ===========================================================
#include "kernel32.h"
#include "api.h"

// Current path search order.
char *SysSearchPath=JTMOS_PATH;
// Current work directory (OBSOLETE).
char SysCWD[100]="/ram";
// Convert alter to multi-thread (TODO!).
FILE *stderr;
FILE *stdout;
FILE *stdin;


// Register a new device.
int registerNewDevice(const char *name,
        void *call,int cs,
        int pid,int type,
        int alias)
{
        //
        return scall(SYS_registernewdevice,
                name,call,cs, pid,type,alias);
}

//
void startMessageBox(void)
{
        //
        scall(SYS_activatemsgbox, 0,0,0, 0,0,0);
}

// Basic postman command poster function.
int Posti(int id,
	void *par1,void *par2,void *par3,void *par4,
	int v1,int v2)
{
/*	int rv;
	POSTMANCMD *p;

	//
	p = malloc(sizeof(POSTMANCMD));
	p->par1 = A2K(par1);
	p->par2 = A2K(par2);
	p->par3 = A2K(par3);
	p->par4 = A2K(par4);
	p->v1 = v1;
	p->v2 = v2;
	p->id = id;
	rv = PostmanPost(p);
	free(p);
	return rv;*/
}

// Search through specified paths for a file.
int SearchThroughPath(char *path, char *name, char *found)
{
	char str[256],str2[256];
	int i,i2,i3,i4,l,fd,sz;

	// Default to nothing found.
	strcpy(found, "");

	// Get path string length.
	l = strlen(path);

	// Go through all chars in the path string.
	for(i=0; i<l; )
	{
		// Skip through all ':' chars.
		for(; path[i]==':' && path[i]!=0; i++);
		if(path[i]==0)	return 1;

		// Grab single path string.
		for(i2=0; path[i]!=':' && path[i]!=0; i++,i2++)
			str[i2] = path[i];
		str[i2]=0;

		// Combine the single path string and the file name.
		sprintf(str2, "%s/%s", str, name);

		// Look for the file.
	        fd = open(str2, O_RDONLY);
	        sz =    lseek(fd, 0, SEEK_END);
	        close(fd);   
	        if(sz>0)
		{
			// Found file.

			// Specify where we found it.
			strcpy(found, str2);
			return 1;
		}
	}

	// Not found.
	return 0;
}

// Copy memory chunk from process to process.
int globalcopy(int srcPID, DWORD srcOffs,
	int dstPID, DWORD dstOffs,
	int amount)
{
	//
	return scall(SYS_globalcopy, srcPID, srcOffs,
		dstPID, dstOffs,
		amount,0);
}

// Copy terminal's contents to a buffer.
int CopyTerminal(int terID, BYTE *buf)
{
        //
        return scall(SYS_copyterminal,
                terID,buf,0, 0,0,0);
}

// Run a command & wait until it exits
int cexec(const char *fname, const char *workDir)
{
	int pid;

        //
        if(!fname)return 1;
        if(!isValidString(fname))return 2;

        // fname, 0 or 1(1 means wait until process exits), rest are zeroes
        scall(SYS_cexec, fname,1,workDir,&pid,GetCurrentTerminal(),0);

	//
	return pid;
}

// Run a process in background
int bgexec(const char *fname, const char *workDir)
{
	int pid;

        //
        if(!fname)return 1;
        if(!isValidString(fname))return 2;

        // fname, 0 or 1(1 means wait until process exits), rest are zeroes
        scall(SYS_cexec, fname,0,workDir,&pid,GetCurrentTerminal(),0);

	//
	return pid;
}

// Execute program and search it from various paths.
// Note: Waits until command is done.
int system(const char *_cmd)
{
        char *p,*s,*e,*es;
        int i,i2,i3,i4,ii,l,ll;
        char str[100],cmd[100];
        int r;

        //
        if(_cmd==NULL)return 1;

        // Get name
        ll = strlen(_cmd);
        for(i=0; i<ll; i++)
        {
                if(_cmd[i]==' ')break;
                str[i] = _cmd[i];
        }
        str[i]=0;
        l = strlen(str);

        // Add extension when needed
        if( strchr(str, '.')==NULL )
        {
                strcat(str, JTMOS_EXE_FMT);
        }

        // Get new file name + merge rest
        // [executable.bin]_[arguments]
        // exe
        strcpy(cmd, str);
        // arguments
        strcat(cmd, _cmd+l);

        //
        printf("LAUNCH '%s'\n", cmd);

        // Search through all adviced paths
        es = SysSearchPath; es+=strlen(SysSearchPath);
        for(ii=0,r=1,p=SysSearchPath; p<es; ii++)
        {
                // Get character
                for(i=0; *p!=';' && *p && i<95; p++,i++) { str[i]=*p; } str[i]=0;
                if(!i || !*p)break;
                p++;
//              printf("TRYING PATH '%s' FOR FILE '%s'\n", str, cmd);
                // cmd = exe+args, str=run path location
                if( !_system(cmd, str) ){r=0; break;}
        }
//      printf("DONE\n");
        return r;
}

//
int _system(const char *cmd, const char *path)
{
        char tmp[100];
        int pid,rv;

        // Set return value to zero
        rv=0;

        // Save current working directory
        getcwd(tmp, 99);

        //
        chdir(path);
        pid = cexec(cmd, tmp);
        if(pid==-1)     { rv=1; goto skipWait; }
        if(!pid)        goto skipWait;

        // Wait until process exits.
        WaitProcessTermination(pid);

skipWait:
        // Change back to old dir.
        chdir(tmp);
        return rv;
}



int remove(const char *path)
{
	return unlink(path);
}
