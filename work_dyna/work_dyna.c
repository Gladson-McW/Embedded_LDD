#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/sysfs.h>
#include <linux/kobject.h>
#include <linux/workqueue.h>
#include <linux/err.h>
#include <linux/smp.h>  // Required for IPI

#define IPI_NO 5  // Define IPI Number

/* Work structure */
static struct work_struct workqueue;

void workqueue_fn(struct work_struct *work);

/* Workqueue Function */
void workqueue_fn(struct work_struct *work)
{
    printk(KERN_INFO "Executing Workqueue Function via IPI\n");
}

/* IPI Handler Function */
static void ipi_handler(void *info)
{
    printk(KERN_INFO "IPI Received - Scheduling Workqueue\n");
    schedule_work(&workqueue);
}

volatile int etx_value = 0;
dev_t dev = 0;
static struct class *dev_class;
static struct cdev etx_cdev;
struct kobject *kobj_ref;

/* Function Prototypes */
static int __init etx_driver_init(void);
static void __exit etx_driver_exit(void);
static int etx_open(struct inode *inode, struct file *file);
static int etx_release(struct inode *inode, struct file *file);
static ssize_t etx_read(struct file *filp, char __user *buf, size_t len, loff_t *off);
static ssize_t etx_write(struct file *filp, const char *buf, size_t len, loff_t *off);

/* Sysfs Functions */
static ssize_t sysfs_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf);
static ssize_t sysfs_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count);

struct kobj_attribute etx_attr = __ATTR(etx_value, 0660, sysfs_show, sysfs_store);

static struct file_operations fops = {
    .owner   = THIS_MODULE,
    .read    = etx_read,
    .write   = etx_write,
    .open    = etx_open,
    .release = etx_release,
};

static ssize_t sysfs_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    printk(KERN_INFO "Sysfs - Read\n");
    return sprintf(buf, "%d", etx_value);
}

static ssize_t sysfs_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
    printk(KERN_INFO "Sysfs - Write\n");
    sscanf(buf, "%d", &etx_value);
    return count;
}

static int etx_open(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "Device File Opened\n");
    return 0;
}

static int etx_release(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "Device File Closed\n");
    return 0;
}

static ssize_t etx_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
    printk(KERN_INFO "Read Function - Sending IPI\n");
    smp_call_function_single(1, ipi_handler, NULL, 1);  // Send IPI to CPU 1
    return 0;
}

static ssize_t etx_write(struct file *filp, const char __user *buf, size_t len, loff_t *off)
{
    printk(KERN_INFO "Write Function\n");
    return 0;
}

static int __init etx_driver_init(void)
{
    if (alloc_chrdev_region(&dev, 0, 1, "etx_Dev") < 0) {
        printk(KERN_INFO "Cannot allocate major number\n");
        return -1;
    }
    printk(KERN_INFO "Major = %d Minor = %d \n", MAJOR(dev), MINOR(dev));

    cdev_init(&etx_cdev, &fops);
    if (cdev_add(&etx_cdev, dev, 1) < 0) {
        printk(KERN_INFO "Cannot add the device\n");
        goto r_class;
    }

    if (IS_ERR(dev_class = class_create("etx_class_word"))) {
        printk(KERN_INFO "Cannot create class\n");
        goto r_class;
    }

    if (IS_ERR(device_create(dev_class, NULL, dev, NULL, "etx_device_word"))) {
        printk(KERN_INFO "Cannot create Device\n");
        goto r_device;
    }

    kobj_ref = kobject_create_and_add("etx_sysfs_word", kernel_kobj);
    if (sysfs_create_file(kobj_ref, &etx_attr.attr)) {
        printk(KERN_INFO "Cannot create sysfs file\n");
        goto r_sysfs;
    }

    INIT_WORK(&workqueue, workqueue_fn);
    printk(KERN_INFO "IPI Workqueue Device Driver Inserted\n");
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

static void __exit etx_driver_exit(void)
{
    kobject_put(kobj_ref);
    sysfs_remove_file(kernel_kobj, &etx_attr.attr);
    device_destroy(dev_class, dev);
    class_destroy(dev_class);
    cdev_del(&etx_cdev);
    unregister_chrdev_region(dev, 1);
    printk(KERN_INFO "IPI Workqueue Device Driver Removed\n");
}

module_init(etx_driver_init);
module_exit(etx_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Gladson");
MODULE_DESCRIPTION("Linux device driver using IPI to trigger Workqueue");
MODULE_VERSION("1.0");
