
###文件的读写



       #includ e <sys/types.h>
       #include <sys/stat.h>
       #include <fcntl.h>

       int open(const char *pathname, int flags);
       int open(const char *pathname, int flags, mode_t mode)
       int creat(const char *pathname, mode_t mode);


       #include <unistd.h>

       ssize_t read(int fd, void *buf, size_t count);


       #include <unistd.h>

       ssize_t write(int fd, const void *buf, size_t count);


       #include <unistd.h>

       int close(int fd);

