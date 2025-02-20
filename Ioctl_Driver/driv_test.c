/* IOCTL Driver Implementation */

/* Header File Inclusion */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h> /* Ioctl User Space Header File Inclusion */



/* Write Value and Read Value */
#define WR_VALUE _IOW('a','a',int32_t*)
#define RD_VALUE _IOR('a','b',int32_t*)



int main()
{
        int fd;
        int32_t value, number;
        printf("**************************************************\n");
        printf("*******Read and Write Kernel Driver (IOCTL)*******\n");
 
        printf("\nOpening IOCTL Driver\n");
        fd = open("/dev/etx_device_ioctl", O_RDWR);
        if(fd < 0) {
                printf("Cannot open device file...\n");
                return 0;
        }
 
        printf("Enter the Value to send\n");
        scanf("%d",&number);
        printf("Writing Value to IOCTL Driver\n");
        ioctl(fd, WR_VALUE, (int32_t*) &number); 
 
        printf("Reading Value from IOCTL Driver\n");
        ioctl(fd, RD_VALUE, (int32_t*) &value);
        printf("Value is %d\n", value);
 
        printf("Closing IOCTL Driver\n");
        close(fd);
}
