#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/kthread.h>
#include <linux/sched.h>
#include <linux/delay.h>
#include <linux/gpio.h>
#include <linux/timekeeping.h>
#include <linux/serial.h>
#include <linux/serial_core.h>
#include <linux/fs.h>
#include <linux/fcntl.h>
#include <linux/gpio/consumer.h> 
#include <linux/ktime.h>
#include <linux/err.h>
#include <linux/printk.h>
/* Device Variables */
dev_t dev = 0;
static struct class *dev_class;
static struct cdev etx_cdev;

/* GPIO Pins */
#define GPIO_PORT_PIN 23

/* UART Device Node */
#define UART_DEVICE "/dev/ttyAMA10" // Change this as per your system (e.g., ttyAMA0 on Raspberry Pi)
#define GPIO_NUMBER 594

static struct task_struct *uart_thread;
static struct task_struct *gpio_thread;

/* File Operations Structure */
static struct file_operations fops =
{
    .owner = THIS_MODULE,
};

/* Kernel Thread Function Prototypes */
int uart_thread_fn(void *pv);
int gpio_thread_fn(void *pv);

int uart_thread_fn(void *pv)
{
    struct file *uart_file;
    char tx_data[] = "Hello from kernel UART\n";
    char rx_data[64] = {0};
    ssize_t read_size, write_size;
    loff_t pos = 0; // File position for read/write

    uart_file = filp_open(UART_DEVICE, O_RDWR | O_NOCTTY, 0);
    if (IS_ERR(uart_file))
    {
        pr_err("UART Thread: Failed to open UART device file\n");
        return -1;
    }

    while (!kthread_should_stop())
    {
        struct timespec64 ts;
        ktime_get_real_ts64(&ts);

        /* Transmit Data */
        write_size = kernel_write(uart_file, tx_data, sizeof(tx_data), &pos);
        if (write_size < 0)
            pr_err("UART Thread: Error writing to UART\n");
        else
            pr_info("UART Thread: Sent Data: %s at %lld.%09ld\n", tx_data, (long long)ts.tv_sec, ts.tv_nsec);

        msleep(500);

        /* Receive Data */
        read_size = kernel_read(uart_file, rx_data, sizeof(rx_data) - 1, &pos);
        if (read_size > 0)
        {
            rx_data[read_size] = '\0'; // Null-terminate the received string
            pr_info("UART Thread: Received Data: %s at %lld.%09ld\n", rx_data, (long long)ts.tv_sec, ts.tv_nsec);
        }

        msleep(1000);
    }

    filp_close(uart_file, NULL);
    return 0;
}

static struct gpio_desc *gpio_desc_gpio23;

/* GPIO Thread Function */
int gpio_thread_fn(void *pv)
{
    struct timespec64 ts;

    /* Request GPIO 23 (Mapped as GPIO 594) */
    gpio_desc_gpio23 = gpiod_get_index(NULL, "gpiochip0", 594, GPIOD_OUT_LOW);

    if (IS_ERR(gpio_desc_gpio23))
    {
        pr_err("Failed to acquire GPIO 594: Error %ld\n", PTR_ERR(gpio_desc_gpio23));
        return PTR_ERR(gpio_desc_gpio23);  // Return actual error code
    }

    while (!kthread_should_stop())
    {
        ktime_get_real_ts64(&ts);
        pr_info("GPIO Thread: Current Time: %lld.%09ld\n", (long long)ts.tv_sec, ts.tv_nsec);

        /* Toggle GPIO */
        gpiod_set_value(gpio_desc_gpio23, 1);
        msleep(500);
        gpiod_set_value(gpio_desc_gpio23, 0);
        msleep(500);
    }

    /* Clean up before exiting */
    if (gpio_desc_gpio23) {
        gpiod_set_value(gpio_desc_gpio23, 0); // Ensure GPIO is turned off
        gpiod_put(gpio_desc_gpio23); // Release GPIO
        gpio_desc_gpio23 = NULL;
    }

    pr_info("GPIO Thread exiting...\n");
    return 0;
}



/* Module init function */
static int __init thre_driver_init(void)
{
    struct device *dev_device;

    /* Allocate Major Number */
    if ((alloc_chrdev_region(&dev, 0, 1, "etx_dev_thre")) < 0)
    {
        pr_err("Cannot allocate major number\n");
        return -1;
    }
    pr_info("Major = %d Minor = %d\n", MAJOR(dev), MINOR(dev));

    /* Create cdev structure */
    cdev_init(&etx_cdev, &fops);

    /* Add character device to the system */
    if ((cdev_add(&etx_cdev, dev, 1)) < 0)
    {
        pr_err("Cannot add the device to the system\n");
        goto r_class;
    }

    /* Create struct class */
    if (IS_ERR(dev_class = class_create("etx_class_thre")))
    {
        pr_err("Cannot create the struct class\n");
        goto r_device;
    }

    /* Create device */
    dev_device = device_create(dev_class, NULL, dev, NULL, "etx_dev_thre");
    if (IS_ERR(dev_device))
    {
        pr_err("Cannot create device\n");
        goto r_device;
    }

    /* Create UART thread */
    uart_thread = kthread_run(uart_thread_fn, NULL, "uart_thread");
    if (IS_ERR(uart_thread))
    {
        pr_err("Cannot create UART thread\n");
        goto r_device;
    }

    /* Create GPIO thread */
    gpio_thread = kthread_run(gpio_thread_fn, NULL, "gpio_thread");
    if (IS_ERR(gpio_thread))
    {
        pr_err("Cannot create GPIO thread\n");
        kthread_stop(uart_thread);
        goto r_device;
    }

    pr_info("Kernel Thread Device Driver Inserted...!!\n");
    return 0;

r_device:
    class_destroy(dev_class);
r_class:
    unregister_chrdev_region(dev, 1);
    cdev_del(&etx_cdev);
    return -1;
}

static void __exit thre_driver_exit(void)
{
    pr_info("Exiting thre_driver module\n");

    /* Check if UART thread is valid before stopping */
    if (uart_thread) {
        kthread_stop(uart_thread);
        pr_info("UART thread stopped\n");
        uart_thread = NULL; // Prevent double stop
    }

    /* Check if GPIO thread is valid before stopping */
    if (gpio_thread) {
        kthread_stop(gpio_thread);
        pr_info("GPIO thread stopped\n");
        gpio_thread = NULL; // Prevent double stop
    } else {
        pr_warn("GPIO thread was never initialized or already stopped.\n");
    }

    pr_info("thre_driver module removed successfully\n");

    device_destroy(dev_class, dev);
    class_destroy(dev_class);
    cdev_del(&etx_cdev);
    unregister_chrdev_region(dev, 1);
    pr_info("Kernel Thread Device Driver Removed... Done!!\n");
}


/* Module Init and Exit */
module_init(thre_driver_init);
module_exit(thre_driver_exit);

/* Module Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Gladson");
MODULE_DESCRIPTION("Kernel Thread Device Driver using Existing UART & GPIO Drivers");
MODULE_VERSION("1.21");
