default: buildcapp

buildcapp:
		gcc -c capp.c
		gcc -o capp \
			capp.o \
			../kernel32/heart/threads/threads_db.o \
			../kernel32/heart/threads/threads.o \
			../kernel32/devsys/devapi.o \
			../kernel32/devsys/devapi_lock.o \
			../kernel32/devsys/devapi_rw.o \
			../kernel32/devsys/devapi_devcall.o \
			../kernel32/devsys/devsys.o \
			../kernel32/heart/threads/AsmTestApp.o \
			../kernel32/heart/threads/exec.o \
			../kernel32/heart/threads/gdtmanipulation.o \
			../kernel32/heart/threads/IdleThread.o \
			../kernel32/heart/threads/launchapp.o \
			../kernel32/heart/threads/LaunchApp.o \
			../kernel32/heart/threads/msgbox.o \
			../kernel32/heart/threads/scheduler.o \
			../kernel32/heart/threads/stack.o \
			../kernel32/heart/threads/threadSwitch.o \
			../kernel32/heart/dataproc/dfunc.o \
			../kernel32/jtmfs/createfat.o \
			../kernel32/jtmfs/dirdbAccess.o \
			../kernel32/jtmfs/dirdbCorruption.o \
			../kernel32/jtmfs/dirdbFind.o \
			../kernel32/jtmfs/dirdbInformation.o \
			../kernel32/jtmfs/dirdb.o \
			../kernel32/jtmfs/dirdbWalk.o \
			../kernel32/jtmfs/dir.o \
			../kernel32/jtmfs/flexCorruption.o \
			../kernel32/jtmfs/flexFat.o \
			../kernel32/jtmfs/fname_translation.o \
			../kernel32/jtmfs/format.o \
			../kernel32/jtmfs/fsaddblocks.o \
			../kernel32/jtmfs/fsadd.o \
			../kernel32/jtmfs/fsdev.o \
			../kernel32/jtmfs/fsdos.o \
			../kernel32/jtmfs/fsfind.o \
			../kernel32/jtmfs/fslast.o \
			../kernel32/jtmfs/fswalk.o \
			../kernel32/jtmfs/isJTMFS.o \
			../kernel32/jtmfs/jtmfat.o \
			../kernel32/jtmfs/jtmfsAccessInit.o \
			../kernel32/jtmfs/jtmfsAccess.o \
			../kernel32/jtmfs/jtmfs.o \
			../kernel32/jtmfs/jtmfsPath.o \
			../kernel32/jtmfs/mount.o \
			../kernel32/jtmfs/ReadChain.o \
			../kernel32/jtmfs/root.o \
			../kernel32/jtmfs/WriteChain.o \
			../kernel32/jtmfs/writeDir.o \
			../kernel32/init/sysglvar.o
