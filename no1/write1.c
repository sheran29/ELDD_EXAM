/*User Space Application*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int8_t write_buf[50];

int main()
{
        int fd;
        char option;
        fd = open("/dev/sema_device_1", O_RDWR);
        if(fd > 0) {
                printf("file opened successfully.. \n");
        }
        else {
                printf("file cannot open..\n");
                return 0;
        }


        printf("Enter the string to write into driver :");
        scanf("%s", write_buf);
    
        write(fd, write_buf,strlen(write_buf)+1);
        printf("Done\n");
                               

   close(fd);
   return 0;
}