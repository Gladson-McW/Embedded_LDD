
/**
 * @file adc18temp_driver.c
 * @brief I2C driver for temperature sensor on Raspberry pi 5  board.
 * 
 * This module implements an I2C-based character driver to interface with 
 * an BMP280 temperature sensor connected to the Raspberry pi 5 board.
 * 
 * @license GPL
 */

#include <linux/moduleparam.h>
static int debug = 0;
static int poll_interval = 1000;

module_param(debug, int, 0644);
MODULE_PARM_DESC(debug, "Enable debug mode (0= off, 1 = on)");

module_param(poll_interval, int, 0644);
MODULE_PARM_DESC(poll_interval, "Polling Interval in Milliseconds");

#define BUFFER_SIZE 25


#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/version.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/i2c.h>
#include <linux/gpio.h>
#include <linux/kernel.h>
#include <linux/miscdevice.h>
#include <linux/i2c-dev.h>
// Define Driver and class names
#define DRIVER_NAME "adc18temp_bmp280"
#define DRIVER_CLASS "adc18temp_bmp280Class"

// I2C adapter and client structures
static struct i2c_adapter *bmp280_i2c_adapter = NULL;
static struct i2c_client *bmp280_i2c_client = NULL;

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Gladson K");
MODULE_DESCRIPTION("I2C-based character driver for temperature sensor on Raspberry Pi 5 Board using BMP280 Sensor in GY-91 Module");

// I2C device parameters
#define I2C_BUS_AVAILABLE 1  // I2C Bus number
#define SLAVE_DEVICE_NAME "ADC18TEMP" // Device name
#define ADC18_SLAVE_ADDRESS 0x76   // I2C slave address for temperature sensor  , for BMP280 Sensor with Raspberry pi use I2C Address 0x76

// I2C device ID table
static const struct i2c_device_id adc18temp_id[] = {
    { SLAVE_DEVICE_NAME, 0 },
    {}
};

// I2C driver structure
static struct i2c_driver adc18temp_driver = {
    .driver = {
        .name = SLAVE_DEVICE_NAME,
        .owner = THIS_MODULE,
    },
    .id_table = adc18temp_id,
};

// I2C board information structure
static struct i2c_board_info adc18temp_i2c_board_info = {
    I2C_BOARD_INFO(SLAVE_DEVICE_NAME, ADC18_SLAVE_ADDRESS)
};

// Device number, class, and character device structure
static dev_t myDeviceNo;
static struct class *myClass=NULL;
static struct cdev myDevice;

// Calibration variables for temperature calculation
uint16_t dig_T1;
int16_t dig_T2, dig_T3;

/**
 * @brief Reads temperature from the sensor and applies calibration.
 * @return Temperature in hundredths of a degree Celsius.
 */
s32 read_temperature(void);
s32 read_temperature(void) {
	
	if(!bmp280_i2c_client) {
	   printk("I2C Client is Null");
	   return -ENODEV;
	}

	int  var1, var2;
	s32 raw_temp;
	s32 d1, d2, d3;
	/* Read Temperature */
	d1 = i2c_smbus_read_byte_data(bmp280_i2c_client, 0xFA);
	d2 = i2c_smbus_read_byte_data(bmp280_i2c_client, 0xFB);
	d3 = i2c_smbus_read_byte_data(bmp280_i2c_client, 0xFC);
	raw_temp = ((d1<<16) | (d2<<8) | d3) >> 4;

	/* Calculate temperature in degree */
	var1 = ((((raw_temp >> 3) - (dig_T1 << 1))) * (dig_T2)) >> 11;

	var2 = (((((raw_temp >> 4) - (dig_T1)) * ((raw_temp >> 4) - (dig_T1))) >> 12) * (dig_T3)) >> 14;
	return (u32) ((var1 + var2) *5 +128) >> 8;
} 

/**
 * @brief Reads data from the device.
 * @param File File descriptor.
 * @param user_buffer Buffer to copy data to.
 * @param count Number of bytes to read.
 * @param offs Offset. 
 * @return Number of bytes read.
 */
static ssize_t driver_read(struct file *File, char *user_buffer, size_t count, loff_t *offs) { 

    if(!File) {
	File=NULL;
	printk(KERN_ALERT "The Driver File has been given indirect value - read not possible");
	return -EINVAL;
    }
    
    if(!user_buffer) {
//	memset(user_buffer, 0, sizeof(*user_buffer));
        printk(KERN_ALERT "User Buffer not been read");
	return -EINVAL;
    }
    if(!offs) {
	*offs = 0;
        printk(KERN_ALERT "Offset value set to null");
	return -EINVAL;
    }
    int to_copy, not_copied, delta;
    char out_string[BUFFER_SIZE];
    int32_t temperature;
    memset(out_string, 0, BUFFER_SIZE);

    to_copy = min(sizeof(out_string), count);
    temperature = read_temperature();
    snprintf(out_string, sizeof(out_string), "%d.%02d\n", temperature / 100, temperature % 100);
    not_copied = copy_to_user(user_buffer, out_string, to_copy);
    delta = to_copy - not_copied;

   if(not_copied==0) { 
    printk("Data has been read sending to write\n");
    return delta;
   }
   else {
	return -EFAULT;
   }
}

/**
 * @brief Writes data to the device.
 * @param File File descriptor.
 * @param user_buffer Buffer containing data to write.
 * @param count Number of bytes to write.
 * @param offs Offset.
 * @return Number of bytes written or negative on failure.
 */
static ssize_t driver_write(struct file *File, const char *user_buffer, size_t count, loff_t *offs) {
    
    if(!File) {
	File=NULL;
        printk(KERN_ALERT "File has not been detected - write is not possible");
        return -EINVAL;
    }
    
    if(user_buffer == NULL) {
//	memset(user_buffer,0,sizeof(*user_buffer));
        printk(KERN_ALERT "User buffer cant be useful due to no passing of values");
	return -EINVAL;
    }
    if(!offs) {
	*offs = 0;
	printk(KERN_ALERT "Offset value need not to be null");
	return -EINVAL;
    }
    char *buffer;  
    buffer = kmalloc(count, GFP_KERNEL);
    if(!buffer)
	return -ENOMEM;

    int not_copied, to_copy;

    to_copy = min(count, sizeof(buffer));
    not_copied = copy_from_user(buffer, user_buffer, to_copy);

    if (not_copied == 0) {
        printk("Received from user: %s\n", buffer);
	kfree(buffer);
        return to_copy;
    } else {
	kfree(buffer);
        return -EFAULT;
    }
}

/**
 * @brief Opens the device.
 * @param deviceFile Pointer to the inode.
 * @param instance Pointer to the file structure.
 * @return 0 on success, negative value on failure.
 */
static int driver_open(struct inode *deviceFile, struct file *instance) {
    if(!deviceFile) {
       printk(KERN_ALERT "Invalid Inode - Check the name/path of the driver in /dev directory\n");
       return -EINVAL;
   }
   if(!instance) {
      printk(KERN_ALERT "Invalid file given - Check whether file generated is valid or not\n");
      return -EINVAL;
   }
    printk("MyDeviceDriver - Open was called\n");
    return 0;
}

/**
 * @brief Closes the device.
 * @param deviceFile Pointer to the inode.
 * @param instance Pointer to the file structure.
 * @return 0 on success, negative value on failure.
 */
static int driver_close(struct inode *deviceFile, struct file *instance) {
   
    if(!deviceFile) {
	printk(KERN_ALERT "Invalid Inode - Check the name/path of Driver");
        return -EINVAL;
    }
    if(!instance) {
        printk(KERN_ALERT "Invalid File - Check file is generated properly");
        return -EINVAL;
    }
    printk("MyDeviceDriver - Close was called\n");
    return 0;
}

/** @brief File operations structure. */
static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = driver_open,
    .release = driver_close,
    .read = driver_read,
    .write = driver_write,
};

/**
 * @brief Initializes the module.
 * @return 0 on success, negative value on failure.
 */
static int __init ModuleInit(void) {
    int ret = -1;
    u8 id;
    printk("MyDeviceDriver - Hello Kernel\n");

    if (alloc_chrdev_region(&myDeviceNo, 0, 1, DRIVER_NAME) < 0) {
        printk("Device Number couldn't be created\n");
        return -1;
    }

    if ((myClass = class_create(DRIVER_CLASS)) == NULL) {
        goto ClassError;
    }

    if (device_create(myClass, NULL, myDeviceNo, NULL, DRIVER_NAME) == NULL) {
        goto FileError;
    }

    cdev_init(&myDevice, &fops);
    if (cdev_add(&myDevice, myDeviceNo, 1) == -1) {
        goto KernelError;
    }

    bmp280_i2c_adapter = i2c_get_adapter(I2C_BUS_AVAILABLE);
    if (bmp280_i2c_adapter) {
        bmp280_i2c_client = i2c_new_client_device(bmp280_i2c_adapter, &adc18temp_i2c_board_info);
        if (!bmp280_i2c_client) {
            pr_err("Failed to create I2C client\n");
            return -ENODEV;
        }

        if (i2c_add_driver(&adc18temp_driver) != -1) {
            ret = 0;
        }
        i2c_put_adapter(bmp280_i2c_adapter);
    }

    
	/* Read Chip ID */
	id = i2c_smbus_read_byte_data(bmp280_i2c_client, 0xD0);
	printk("ID: 0x%x\n", id);

	/* Read Calibration Values */
	dig_T1 = i2c_smbus_read_word_data(bmp280_i2c_client, 0x88);
	dig_T2 = i2c_smbus_read_word_data(bmp280_i2c_client, 0x8a);
	dig_T3 = i2c_smbus_read_word_data(bmp280_i2c_client, 0x8c);

	if(dig_T2 > 32767)
		dig_T2 -= 65536;

	if(dig_T3 > 32767)
		dig_T3 -= 65536;

	/* Initialice the sensor */
	i2c_smbus_write_byte_data(bmp280_i2c_client, 0xf5, 5<<5);
	i2c_smbus_write_byte_data(bmp280_i2c_client, 0xf4, ((5<<5) | (5<<2) | (3<<0)));
	return ret;

KernelError:
    device_destroy(myClass, myDeviceNo);
FileError:
    class_destroy(myClass);
ClassError:
    unregister_chrdev_region(myDeviceNo, 1);
    return -1;
}

/**
 * @brief Cleans up module resources.
 */
static void __exit ModuleExit(void) {
    printk("MyDeviceDriver - Goodbye, Kernel!\n");
    i2c_unregister_device(bmp280_i2c_client);
    i2c_del_driver(&adc18temp_driver);
    cdev_del(&myDevice);
    device_destroy(myClass, myDeviceNo);
    class_destroy(myClass);
    unregister_chrdev_region(myDeviceNo, 1);
}

module_init(ModuleInit);
module_exit(ModuleExit);
