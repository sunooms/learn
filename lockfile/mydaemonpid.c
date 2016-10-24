#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#define LOCKFILE "/var/run/mydaemon.pid"
#define LOCKMOD  (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

/* set advisory lock on file*/
/* advisory lock 协同锁  mandatory lock 强制锁*/
int lockfile(int fd)
{
    struct flock fl;

    fl.l_type = F_WRLCK;    /*write lock. type of lock: F_RDLCK, F_WRLCK, F_UNLCK*/
    fl.l_whence = SEEK_SET; /* how to interpret l_start:SEEK_SET, SEEK_CUR, SEEK_END*/
    fl.l_start = 0;         /* starting offset for lock*/
    fl.l_len = 0;           /* lock the whole file. number of bytes to lock */

    return (fcntl(fd, F_SETLK, &fl));
}

int already_running(const char *filename)
{
    int fd;
    char buf[16];

    fd = open(filename, O_RDWR|O_CREAT, LOCKMOD);
    if(fd < 0)
    {
        printf("can't open %s\n", filename);
        return 1;
    }

    // first get file lock
    if(lockfile(fd) == -1)
    {
        if(errno == EACCES || errno == EAGAIN)
        {
            printf("file:%s already locked\n", filename);
            close(fd);
            return 1;
        }
        printf("can't lock %s:%m\n", filename);
        return 1;
    }

    /*write process's pid*/
    ftruncate(fd, 0);
    sprintf(buf, "%ld", (long)getpid());
    write(fd, buf, strlen(buf) + 1);
    return 0;
}

int main()
{
    if(already_running(LOCKFILE))
        return 0;

    printf("start main..\n");
    sleep(100);
    printf("main done!\n");
    return 0;
}
