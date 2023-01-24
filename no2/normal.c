/*User Space Application*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define WR_DATA _IOW('a','a',int32_t *)
#define RD_DATA _IOR('a','b',int32_t *)


int8_t write_buf[1024];
int8_t read_buf[1024];

int main()
{
        int fd;
        char cmd;
        fd = open("/dev/kernel_module", O_RDWR);       // opens the driver 
        if(fd > 0) 
                printf("Device File Opened successfully in the path /dev/kernel_module...\n");
        
        else 
                printf("Cannot open device file...\n");
           
       // while(1)
		//{
	 printf("Enter the operation to follow:");
	 scanf("%c",&cmd);
                switch(cmd) {
                        case 'c':
                                printf("Enter the value:");
                                scanf("  %[^\t\n]s", write_buf);
                                printf("Data Writing ...");
                                ioctl(fd, WR_DATA,write_buf);
                                printf("Done!\n");
                                break;
                        case 'g':
                                printf("Data Reading ...");
                                ioctl(fd, RD_DATA,read_buf);
                                printf("Done!\n\n");
                                printf("Data = %s\n\n", read_buf);
                                break;
                      
                        default:
                                printf("Enter Valid option = %c\n",cmd);
                                break;
                }
        //}
        close(fd);
        return 0;
}
