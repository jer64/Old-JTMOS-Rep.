#ifndef __INCLUDED_DIRECTFILEACCESS_H__
#define __INCLUDED_DIRECTFILEACCESS_H__

//
#define fexist exist
int renamefile(const char *oldpath, const char *newpath);
int createfile(const char *path);
int deletefile(const char *path);
int readfile(char *path, char *buf, int offset, int count);
int writefile(const char *path,char *buf,int offset,int count);
int getfilesize(const char *path);
int addfileblocks(const char *path, int count);
int setsize(const char *path, int newsize);
int exist(const char *path);
int findfirst(const char *path, void *ffblk);
int findnext(int fd, void *ffblk);
int close(int fd);
void getfiledatablock(const char *path);
int createdirectory(const char *path);
void formatdrive(int dnr);
void changedirectory(const char *path);
void isdriveready(int dnr);
int getcwd(const char *path, int max_len);
int open(const char *pathname, int flags);
int creat(const char *pathname, int flags);
int close(int fd);
int read(int fd, void *buf, int count);
int write(int fd, void *buf, int count);
int lseek(int fd, int offset, int whence);
int readdir(unsigned int fd, void *dirp,
          unsigned int count);
int mkdir(const char *pathname, int mode);
int rmdir(const char *pathname);
int unlink(const char *pathname);
int chdir(const char *pathname);
int mkfs(const char *path);
int sync(void);
int syncfs(int fd);

#endif