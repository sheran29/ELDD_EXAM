#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int8_t read_buf[50];

int main()
{
       
	int fd;
    fd =open("/dev/sema_device_1",O_RDWR);
    if(fd>0)
    {
        printf("file open successfullly...\n");
    }
    else
    {
        printf("file cannot open..\n");
        return 0;
    }
    printf("reading_1.....");
    read(fd,read_buf,50);
    printf("Data = %s\n\n", read_buf);
    close(fd);
    return 0;

}
