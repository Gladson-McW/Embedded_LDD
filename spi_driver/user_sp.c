#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <stdint.h>
#include <string.h>

#define SPI_DEVICE "/dev/spidev10.0"  // Change according to your setup
#define SPI_SPEED 500000             // 500 kHz SPI speed
#define SPI_MODE SPI_MODE_0          // SPI mode
#define SPI_BITS_PER_WORD 8          // Bits per word

int main() {
    int fd;
    uint8_t tx_buf[2], rx_buf[2];
    struct spi_ioc_transfer spi;

    // Open the SPI device
    fd = open(SPI_DEVICE, O_RDWR);
    if (fd < 0) {
        perror("Failed to open SPI device");
        return -1;
    }
    printf("SPI device opened successfully\n");

    // Configure SPI settings
    uint32_t speed = SPI_SPEED;
    uint8_t mode = SPI_MODE;
    uint8_t bits = SPI_BITS_PER_WORD;
    
    ioctl(fd, SPI_IOC_WR_MODE, &mode);
    ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
    ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);

    // Prepare SPI transfer to read temperature register (e.g., 0xFA for BMP280)
    tx_buf[0] = 0xFA;  // BMP280 temperature register address
    tx_buf[1] = 0x00;  // Dummy byte

    memset(&spi, 0, sizeof(spi));
    spi.tx_buf = (unsigned long)tx_buf;
    spi.rx_buf = (unsigned long)rx_buf;
    spi.len = 2;
    spi.speed_hz = SPI_SPEED;
    spi.bits_per_word = SPI_BITS_PER_WORD;
    spi.cs_change = 0;

    // Send & receive data
    if (ioctl(fd, SPI_IOC_MESSAGE(1), &spi) < 0) {
        perror("SPI transfer failed");
        close(fd);
        return -1;
    }

    // Print received temperature data
    printf("Received Data: 0x%02X 0x%02X\n", rx_buf[0], rx_buf[1]);

    // Close the SPI device
    close(fd);
    printf("SPI device closed\n");

    return 0;
}

