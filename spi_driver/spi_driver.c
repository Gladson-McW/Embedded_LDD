/**
 * @file adc18temp_driver.c
 * @brief SPI driver for BMP280 temperature sensor on Raspberry Pi 5 board.
 * 
 * This module implements an SPI-based character driver to interface with 
 * a BMP280 temperature sensor connected to the Raspberry Pi 5 board.
 * 
 * @license GPL
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/version.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/spi/spi.h>
#include <linux/kernel.h>
#include <linux/miscdevice.h>

// Define Driver Name
#define DRIVER_NAME "bmp280temp"
#define SPI_SPEED 1000000

// SPI device structure
static struct spi_device *bmp280_spi_device = NULL;

// BMP280 Calibration Variables
static u16 dig_T1;
static s16 dig_T2, dig_T3;

/**
 * @brief Reads BMP280 calibration data from the sensor.
 */
static void bmp280_read_calibration(void) {
    if (!bmp280_spi_device) return;

    u8 tx_buf[7] = {0x88 | 0x80, 0, 0, 0, 0, 0, 0}; // Read calibration registers
    u8 rx_buf[7] = {0};

    struct spi_transfer t = {
        .tx_buf = tx_buf,
        .rx_buf = rx_buf,
        .len = 7,
        .speed_hz = SPI_SPEED,
        .bits_per_word = 8,
    };

    struct spi_message m;
    spi_message_init(&m);
    spi_message_add_tail(&t, &m);
    spi_sync(bmp280_spi_device, &m);

    dig_T1 = (rx_buf[1] << 8) | rx_buf[0];
    dig_T2 = (rx_buf[3] << 8) | rx_buf[2];
    dig_T3 = (rx_buf[5] << 8) | rx_buf[4];
}

/**
 * @brief Reads temperature from the BMP280 sensor via SPI.
 * @return Temperature in hundredths of a degree Celsius.
 */
static u32 read_temperature(void) {
    if (!bmp280_spi_device) {
        printk(KERN_ERR "BMP280: SPI Device is NULL\n");
        return -ENODEV;
    }

    u8 tx_buf[3] = {0xFA | 0x80, 0, 0}; // Read command for temperature
    u8 rx_buf[3] = {0};

    struct spi_transfer t = {
        .tx_buf = tx_buf,
        .rx_buf = rx_buf,
        .len = 3,
        .speed_hz = SPI_SPEED,
        .bits_per_word = 8,
    };

    struct spi_message m;
    spi_message_init(&m);
    spi_message_add_tail(&t, &m);
    spi_sync(bmp280_spi_device, &m);

    s32 raw_temp = ((rx_buf[0] << 16) | (rx_buf[1] << 8) | rx_buf[2]) >> 4;
    int var1 = ((((raw_temp >> 3) - (dig_T1 << 1))) * (dig_T2)) >> 11;
    int var2 = (((((raw_temp >> 4) - dig_T1) * ((raw_temp >> 4) - dig_T1)) >> 12) * dig_T3) >> 14;
    return (u32)((var1 + var2) * 5 + 128) >> 8; // Temperature in 0.01°C
}

/**
 * @brief Reads temperature data when the user reads from /dev/bmp280temp.
 */
static ssize_t bmp280_read(struct file *file, char __user *buf, size_t count, loff_t *ppos) {
    u32 temp = read_temperature();
    char temp_str[10];
    int len = snprintf(temp_str, sizeof(temp_str), "%u\n", temp);

    return simple_read_from_buffer(buf, count, ppos, temp_str, len);
}

// Character device file operations
static const struct file_operations bmp280_fops = {
    .owner = THIS_MODULE,
    .read = bmp280_read,
};

// Registering the character device
static struct miscdevice bmp280_misc = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = "bmp280temp",
    .fops = &bmp280_fops,
};

/**
 * @brief SPI Probe function - called when the SPI device is registered.
 */
static int spi_probe(struct spi_device *spi) {
    bmp280_spi_device = spi;
    bmp280_read_calibration();

    if (misc_register(&bmp280_misc)) {
        printk(KERN_ERR "BMP280: Failed to register misc device\n");
        return -ENODEV;
    }

    printk(KERN_INFO "BMP280: SPI device registered successfully\n");
    return 0;
}

/**
 * @brief SPI Remove function - called when the SPI device is unregistered.
 */
static void spi_remove(struct spi_device *spi) {
    misc_deregister(&bmp280_misc);
    bmp280_spi_device = NULL;
    printk(KERN_INFO "BMP280: SPI device removed\n");
}

// SPI Driver structure
static struct spi_driver adc18temp_driver = {
    .driver = {
        .name = "adc18temp_spi",
        .owner = THIS_MODULE,
    },
    .probe = spi_probe,
    .remove = spi_remove,
};

// Module Init & Exit
static int __init ModuleInit(void) {
    printk(KERN_INFO "Initializing BMP280 SPI Driver\n");
    return spi_register_driver(&adc18temp_driver);
}

static void __exit ModuleExit(void) {
    printk(KERN_INFO "Exiting BMP280 SPI Driver\n");
    spi_unregister_driver(&adc18temp_driver);
}

module_init(ModuleInit);
module_exit(ModuleExit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Gladson K");
MODULE_DESCRIPTION("SPI-based character driver for BMP280 Sensor on Raspberry Pi 5");

