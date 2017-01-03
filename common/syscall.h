//
// Created by ben on 28/12/16.
//

#ifndef COMMON_SYSCALL_H
#define COMMON_SYSCALL_H

//mmap

#define PROT_READ       0x1             /* page can be read */
#define PROT_WRITE      0x2             /* page can be written */
#define PROT_EXEC       0x4             /* page can be executed */
#define PROT_SEM        0x8             /* page may be used for atomic ops */
#define PROT_NONE       0x0             /* page can not be accessed */
#define PROT_GROWSDOWN  0x01000000      /* mprotect flag: extend change to start of growsdown vma */
#define PROT_GROWSUP    0x02000000      /* mprotect flag: extend change to end of growsup vma */

#define MAP_SHARED      0x01            /* Share changes */
#define MAP_PRIVATE     0x02            /* Changes are private */
#define MAP_TYPE        0x0f            /* Mask for type of mapping */
#define MAP_FIXED       0x10            /* Interpret addr exactly */
#define MAP_ANONYMOUS   0x20            /* don't use a file */

#define MAP_GROWSDOWN   0x0100          /* stack-like segment */

#define MAP_POPULATE	0x8000		/* populate (prefault) pagetables */
#define MAP_NONBLOCK	0x10000		/* do not block on IO */

#define MAP_FAILED ((void*)-1)

//file

typedef struct file {
    int fd;
    uint64_t inode;
} file_t;

//stat

typedef struct timespec
{
    int64_t tv_sec;		/* Seconds.  */
    int64_t tv_nsec;	/* Nanoseconds.  */
} timespec_t;

typedef struct stat {
    uint64_t  st_dev;     /* ID of device containing file */
    uint64_t  st_ino;     /* inode number */
    uint64_t  st_nlink;   /* number of hard links */
    uint32_t   st_mode;    /* protection */
    uint32_t     st_uid;     /* user ID of owner */
    uint32_t     st_gid;     /* group ID of owner */
    unsigned int		__pad0;
    uint64_t     st_rdev;    /* device ID (if special file) */
    int64_t    st_size;    /* total size, in bytes */
    int64_t st_blksize; /* blocksize for file system I/O */
    int64_t  st_blocks;  /* number of 512B blocks allocated */
    timespec_t    st_atime;   /* time of last access */
    timespec_t    st_mtime;   /* time of last modification */
    timespec_t    st_ctime;   /* time of last status change */
} stat_t;

//errno

#define EPERM            1      /* Operation not permitted */
#define ENOENT           2      /* No such file or directory */
#define ESRCH            3      /* No such process */
#define EINTR            4      /* Interrupted system call */
#define EIO              5      /* I/O error */
#define ENXIO            6      /* No such device or address */
#define E2BIG            7      /* Argument list too long */
#define ENOEXEC          8      /* Exec format error */
#define EBADF            9      /* Bad file number */
#define ECHILD          10      /* No child processes */
#define EAGAIN          11      /* Try again */
#define ENOMEM          12      /* Out of memory */
#define EACCES          13      /* Permission denied */
#define EFAULT          14      /* Bad address */
#define ENOTBLK         15      /* Block device required */
#define EBUSY           16      /* Device or resource busy */
#define EEXIST          17      /* File exists */
#define EXDEV           18      /* Cross-device link */
#define ENODEV          19      /* No such device */
#define ENOTDIR         20      /* Not a directory */
#define EISDIR          21      /* Is a directory */
#define EINVAL          22      /* Invalid argument */
#define ENFILE          23      /* File table overflow */
#define EMFILE          24      /* Too many open files */
#define ENOTTY          25      /* Not a typewriter */
#define ETXTBSY         26      /* Text file busy */
#define EFBIG           27      /* File too large */
#define ENOSPC          28      /* No space left on device */
#define ESPIPE          29      /* Illegal seek */
#define EROFS           30      /* Read-only file system */
#define EMLINK          31      /* Too many links */
#define EPIPE           32      /* Broken pipe */
#define EDOM            33      /* Math argument out of domain of func */
#define ERANGE          34      /* Math result not representable */

#endif //COMMON_SYSCALL_H