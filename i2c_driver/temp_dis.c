
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define DEVICE_PATH "/dev/adc18temp_bmp280"

int main() {
    int fd;
    char buffer[20];
    ssize_t bytesRead;
    
    printf("Opening the device\n");

    // Open the device file for reading
    fd = open(DEVICE_PATH, O_RDONLY);
    if (fd < 0) {
        perror("Failed to open the device");
        return -1;
    }
    printf("Device Opened Successfully\n");
    // Read the temperature value from the device
    bytesRead = read(fd, buffer, sizeof(buffer)-1);
    if (bytesRead < 0) {
        perror("Failed to read the device");
        close(fd);
        return -1;
    }
    printf("Read from the device completed successfully\n");

    // Null-terminate the buffer and print the temperature
    buffer[bytesRead] = '\0';
    printf("Temperature in Degree Celsius: %s \n", buffer);

    // Close the device file
    printf("Closing the device\n");
    close(fd);
    

    printf("Program Completed Successfully\n");
    return 0;
}
