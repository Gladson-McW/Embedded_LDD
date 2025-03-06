/**
 * @file bmp280_user.c
 * @brief User-space program to read temperature data from BMP280 over SPI.
 * 
 * This program communicates with the BMP280 sensor using the SPIdev interface 
 * (`/dev/spidev10.0`) and retrieves the temperature.
 * 
 * Compile: gcc bmp280_user.c -o bmp280_user
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <string.h>
#include <errno.h>

#define SPI_DEVICE "/dev/spidev10.0"  // Change this based on your SPI configuration
#define SPI_SPEED 1000000             // 1MHz SPI speed
#define TEMP_REGISTER 0xFA | 0x80     // BMP280 temperature register (0xFA) with read bit (0x80)

int read_temperature(int fd) {
    uint8_t tx_buf[3] = {TEMP_REGISTER, 0, 0};
    uint8_t rx_buf[3] = {0};
    
    struct spi_ioc_transfer spi_transfer = {
        .tx_buf = (unsigned long)tx_buf,
        .rx_buf = (unsigned long)rx_buf,
        .len = 3,
        .speed_hz = SPI_SPEED,
        .bits_per_word = 8,
        .delay_usecs = 0,
    };

    if (ioctl(fd, SPI_IOC_MESSAGE(1), &spi_transfer) < 0) {
        perror("SPI transfer failed");
        return -1;
    }

    // Convert raw temperature value
    int32_t raw_temp = ((rx_buf[0] << 16) | (rx_buf[1] << 8) | rx_buf[2]) >> 4;
    return raw_temp;
}

int main() {
    int fd = open(SPI_DEVICE, O_RDWR);
    if (fd < 0) {
        perror("Failed to open SPI device");
        return EXIT_FAILURE;
    }

    int32_t raw_temp = read_temperature(fd);
    if (raw_temp < 0) {
        close(fd);
        return EXIT_FAILURE;
    }

    printf("Raw Temperature Data: %d\n", raw_temp);
    
    // Convert raw value to Celsius (based on BMP280 calibration formula)
    float temperature = raw_temp / 51200.0;
    printf("Temperature: %.2f °C\n", temperature);

    close(fd);
    return EXIT_SUCCESS;
}

