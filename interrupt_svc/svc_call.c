/* Inter Process Interrupt and SVC Based Software Interrupt in Raspberry pi 5 Board */

/* Include All the Header Files */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/sysfs.h> 
#include <linux/kobject.h> 
#include <linux/interrupt.h>
#include <linux/smp.h>  // For IPI functions
#include <linux/err.h>
#include <asm/io.h>

#define IPI_TEST 2  // Arbitrary IPI number for testing

static void ipi_handler(void *info) {
   printk(KERN_INFO "IPI Received: Core %d\n", smp_processor_id());
}

//static irqreturn_t irq_handler(int irq, void *dev_id) {
   // printk(KERN_INFO "IRQ Handler Called: IRQ %d\n", irq);
  //  return IRQ_HANDLED;
//}

volatile int etx_value = 0;
dev_t dev = 0;
static struct class *dev_class;
static struct cdev etx_cdev;
struct kobject *kobj_ref;

static int __init etx_driver_init(void);
static void __exit etx_driver_exit(void);

static int etx_open(struct inode *inode, struct file *file);
static int etx_release(struct inode *inode, struct file *file);
static ssize_t etx_read(struct file *filp, char __user *buf, size_t len, loff_t *off);
static ssize_t etx_write(struct file *filp, const char *buf, size_t len, loff_t *off);

static struct file_operations fops = {
    .owner   = THIS_MODULE,
    .read    = etx_read,
    .write   = etx_write,
    .open    = etx_open,
    .release = etx_release,
};

static int etx_open(struct inode *inode, struct file *file) {
    printk(KERN_INFO "Device File Opened\n");
    return 0;
}

static int etx_release(struct inode *inode, struct file *file) {
    printk(KERN_INFO "Device File Closed\n");
    return 0;
}

static ssize_t etx_read(struct file *filp, char __user *buf, size_t len, loff_t *off) {
    printk(KERN_INFO "Read function - Triggering SVC and IPI\n");
    
    // Trigger software interrupt (SVC)
//    asm volatile("hvc #42");

    // Trigger an Inter-Processor Interrupt (IPI)
    smp_call_function_single(1, ipi_handler, NULL, 1); // Send IPI to Core 1

    return 0;
}

static ssize_t etx_write(struct file *filp, const char __user *buf, size_t len, loff_t *off) {
    printk(KERN_INFO "Write Function\n");
    return len;
}

static int __init etx_driver_init(void) {
    if (alloc_chrdev_region(&dev, 0, 1, "etx_Dev_int") < 0) {
        printk(KERN_INFO "Cannot allocate major number\n");
        return -1;
    }
    printk(KERN_INFO "Major = %d Minor = %d\n", MAJOR(dev), MINOR(dev));

    cdev_init(&etx_cdev, &fops);
    if (cdev_add(&etx_cdev, dev, 1) < 0) {
        printk(KERN_INFO "Cannot add the device to the system\n");
        goto r_class;
    }

    if (IS_ERR(dev_class = class_create("etx_class_int"))) {
        printk(KERN_INFO "Cannot create struct class\n");
        goto r_class;
    }

    if (IS_ERR(device_create(dev_class, NULL, dev, NULL, "etx_device_int"))) {
        printk(KERN_INFO "Cannot create the Device\n");
        goto r_device;
    }

    kobj_ref = kobject_create_and_add("etx_sysfs_int", kernel_kobj);
    printk(KERN_INFO "InterruptDevice Driver Inserted\n");
    return 0;

r_device:
    class_destroy(dev_class);
r_class:
    unregister_chrdev_region(dev, 1);
    cdev_del(&etx_cdev);
    return -1;
}

static void __exit etx_driver_exit(void) {
    kobject_put(kobj_ref);
    device_destroy(dev_class, dev);
    class_destroy(dev_class);
    cdev_del(&etx_cdev);
    unregister_chrdev_region(dev, 1);
    printk(KERN_INFO "Interrupt Device Driver Removed\n");
}

module_init(etx_driver_init);
module_exit(etx_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Gladson");
MODULE_DESCRIPTION("Interrupts Device Driver using SVC and IPI");
MODULE_VERSION("2.1");
