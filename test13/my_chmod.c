#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <errno.h>


int main(int argc, char* argv[])
{
    struct stat buf;

    //检查参数个数
    if(argc != 2) {
        printf("Usage； my_stat <filename>\n");
        exit(0);
    }

    //获取文件属性
    if(stat(argv[1],&buf) == -1) {
        perror("stat；");
        exit(1);
    }

    //打印文件的属性
    printf("device is: %ld\n",buf.st_dev);
    printf("inode is；%ld\n",buf.st_ino);
    printf("mode is:%o\n",buf.st_mode);
    printf("number of hard links is:%ld\n",buf.st_nlink);
    printf("user ID of owner is；%d\n",buf.st_uid);
    printf("group ID of owner is:%d\n",buf.st_gid);
    printf("device type (if inode device) is:%ld\n",buf.st_rdev);
    printf("total size, in bytes is: %ld\n",buf.st_size);
    printf("blocksize for filessystem I/O is； %ld\n",buf.st_blksize);
    printf("number of blocks allocated is: %ld\n",buf.st_blocks);
    printf("time of last access is: %s",ctime(&buf.st_atim.tv_sec));
    printf("time of last modification is:%s",ctime(&buf.st_mtim.tv_sec));
    printf("time of last chage is: %s",ctime(&buf.st_ctim.tv_sec));

    return 0;
}