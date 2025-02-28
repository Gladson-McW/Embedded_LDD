#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/slab.h>                 // kmalloc()
#include <linux/uaccess.h>              // copy_to/from_user()
#include <linux/sysfs.h> 
#include <linux/kobject.h> 
#include <linux/workqueue.h>            // Required for workqueues
#include <linux/err.h>
#include <linux/smp.h>                  // Required for IPI

#define IPI_NO 5                        // Define IPI Number (Change if needed)

void workqueue_fn(struct work_struct *work);
/* Creating work by Static Method */
DECLARE_WORK(workqueue, workqueue_fn);

/* Workqueue Function */
void workqueue_fn(struct work_struct *work)
{
    printk(KERN_INFO "Executing Workqueue Function\n");
}

/* IPI Handler */
static void ipi_handler(void *info)
{
    printk(KERN_INFO "IPI Received on Core %d\n", smp_processor_id());
    schedule_work(&workqueue);  // Schedule workqueue
}

volatile int etx_value = 0;

dev_t dev = 0;
static struct class *dev_class;
static struct cdev etx_cdev;
struct kobject *kobj_ref;

/*
** Function Prototypes
*/
static int __init etx_driver_init(void);
static void __exit etx_driver_exit(void);

/*************** Driver Functions **********************/
static int etx_open(struct inode *inode, struct file *file);
static int etx_release(struct inode *inode, struct file *file);
static ssize_t etx_read(struct file *filp, char __user *buf, size_t len, loff_t *off);
static ssize_t etx_write(struct file *filp, const char *buf, size_t len, loff_t *off);

/*************** Sysfs Functions **********************/
static ssize_t sysfs_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf);
static ssize_t sysfs_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count);

struct kobj_attribute etx_attr = __ATTR(etx_value, 0660, sysfs_show, sysfs_store);

/*
** File operation structure
*/
static struct file_operations fops =
{
    .owner          = THIS_MODULE,
    .read           = etx_read,
    .write          = etx_write,
    .open           = etx_open,
    .release        = etx_release,
};

/*
** Sysfs Read Function
*/ 
static ssize_t sysfs_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    printk(KERN_INFO "Sysfs - Read!!!\n");
    return sprintf(buf, "%d", etx_value);
}

/*
** Sysfs Write Function
*/
static ssize_t sysfs_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
    printk(KERN_INFO "Sysfs - Write!!!\n");
    sscanf(buf, "%d", &etx_value);
    return count;
}

/*
** Open Function
*/  
static int etx_open(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "Device File Opened...!!!\n");
    return 0;
}

/*
** Release Function
*/  
static int etx_release(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "Device File Closed...!!!\n");
    return 0;
}

/*
** Read Function - Triggers an IPI
*/
static ssize_t etx_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
    printk(KERN_INFO "Read function - Sending IPI to all CPUs\n");

    /* Send IPI to all CPUs except the sender */
    smp_call_function(ipi_handler, NULL, 1);

    return 0;
}

/*
** Write Function
*/
static ssize_t etx_write(struct file *filp, const char __user *buf, size_t len, loff_t *off)
{
    printk(KERN_INFO "Write Function\n");
    return len;
}

/*
** Module Init Function
*/
static int __init etx_driver_init(void)
{
    /* Allocating Major Number */
    if ((alloc_chrdev_region(&dev, 0, 1, "etx_Dev_wors")) < 0) {
        printk(KERN_INFO "Cannot allocate major number\n");
        return -1;
    }
    printk(KERN_INFO "Major = %d Minor = %d \n", MAJOR(dev), MINOR(dev));

    /* Creating cdev structure */
    cdev_init(&etx_cdev, &fops);

    /* Adding character device to the system */
    if ((cdev_add(&etx_cdev, dev, 1)) < 0) {
        printk(KERN_INFO "Cannot add the device to the system\n");
        goto r_class;
    }

    /* Creating struct class */
    if (IS_ERR(dev_class = class_create("etx_class_wors"))) {
        printk(KERN_INFO "Cannot create the struct class\n");
        goto r_class;
    }

    /* Creating device */
    if (IS_ERR(device_create(dev_class, NULL, dev, NULL, "etx_device_wors"))) {
        printk(KERN_INFO "Cannot create the Device\n");
        goto r_device;
    }

    /* Creating a directory in /sys/kernel/ */
    kobj_ref = kobject_create_and_add("etx_sysfs_wors", kernel_kobj);

    /* Creating sysfs file for etx_value */
    if (sysfs_create_file(kobj_ref, &etx_attr.attr)) {
        printk(KERN_INFO "Cannot create sysfs file\n");
        goto r_sysfs;
    }

    printk(KERN_INFO "Device Driver Inserted... Done!!!\n");
    return 0;

r_sysfs:
    kobject_put(kobj_ref);
    sysfs_remove_file(kernel_kobj, &etx_attr.attr);

r_device:
    class_destroy(dev_class);

r_class:
    unregister_chrdev_region(dev, 1);
    cdev_del(&etx_cdev);
    return -1;
}

/*
** Module Exit Function
*/ 
static void __exit etx_driver_exit(void)
{
    kobject_put(kobj_ref);
    sysfs_remove_file(kernel_kobj, &etx_attr.attr);
    device_destroy(dev_class, dev);
    class_destroy(dev_class);
    cdev_del(&etx_cdev);
    unregister_chrdev_region(dev, 1);

    printk(KERN_INFO "Workqueue Static Device Driver Removed... Done!!!\n");
}

/* Module Initialization and Exit */
module_init(etx_driver_init);
module_exit(etx_driver_exit);

/* Module Description and Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Gladson");
MODULE_DESCRIPTION("Workqueue Static Simple Linux device driver using IPI (Inter-Processor Interrupts)");
MODULE_VERSION("2.0");
