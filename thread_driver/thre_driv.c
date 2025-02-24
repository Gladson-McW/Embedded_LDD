/* Implementation of Kernel Thread Driver */

/* Include Kernel Thread Driver */

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
#include <linux/err.h>

dev_t dev = 0;
static struct class *dev_class;
static struct cdev etx_cdev;

static int __init thre_driver_init(void);
static void __exit thre_driver_exit(void);

static struct task_struct *etx_threads;

/* Function Prototypes */

static int thre_open(struct inode *inode, struct file *file);
static int thre_release(struct inode *inode, struct file *file);
static ssize_t thre_read(struct file *filp, char __user *buf, size_t len, loff_t *off);
static ssize_t thre_write(struct file *filp, const char *buf, size_t len, loff_t *off);

/* Thread Function */

int thread_function(void *pv);

int thread_function(void *pv)
{
    int i=0;
    while(!kthread_should_stop()) {
        pr_info("In Own Thread Function %d\n", i++);
        msleep(1000);
    }
    return 0;
}

/* File Operation Structure */

static struct file_operations fops = 
{
        .owner         = THIS_MODULE,
        .read          = thre_read,
        .write         = thre_write,
        .open          = thre_open,
        .release       = thre_release,
};

/* This Function will be called when we open Device File */

static int thre_open(struct inode *inode, struct file *file)
{
    pr_info("Kernel Thread Device File Opened....!!!!\n");
    return 0;
}

/* This Function will be called when we close Device File */

static int thre_release(struct inode *inode, struct file *file)
{
    pr_info("Kernel Thread Device File Closed....!!!!\n");
    return 0;
}

/* Read Device Files */

static ssize_t thre_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
    pr_info("Kernel Thread Read Function\n");
    return 0;
}

/* Write Device Files */

static ssize_t thre_write(struct file *filp, const char *buf, size_t len, loff_t *off)
{
    pr_info("Kernel Thread Write Function\n");
    return len;
}

/* Module Init Function */
static int __init thre_driver_init(void)
{
        /* Allocating Major Number */
        if((alloc_chrdev_region(&dev,0,1,"etx_dev_thre"))<0){
            pr_err("Cannot allocate Major Number\n");
            return -1;
        }
        pr_info("Major = %d Minor = %d \n", MAJOR(dev),MINOR(dev));

        /* Creating cdev structure */
        cdev_init(&etx_cdev, &fops);

        /* Adding Character Device to the System */
        if((cdev_add(&etx_cdev,dev,1)) < 0){
            pr_err("Cannot add the device to the system\n");
            goto r_class;
        }

        /* Creating struct class */
        if(IS_ERR(dev_class= class_create("etx_class_thre"))){
            pr_err("Cannot create the struct class\n");
            goto r_device;
        }

        etx_threads = kthread_create(thread_function,NULL,"etx_device_thre");
        if(etx_threads) {
            wake_up_process(etx_threads);
        } 
        else {
            pr_err("Cannot create kthread\n");
            goto r_device;
        }
#if 0
        /* Use the Method to create and run the thread */
        etx_threads = kthread_run(thread_function,NULL,"etx_device_thre");
        if(etx_threads) {
            pr_info("Kthread Created Successfully....\n");
        }
        else {
            pr_err("Cannot create kthread\n");
            goto r_device;
        }
#endif
        pr_info("Kernel Thread Device Driver Insert Done ...!!\n");
        return 0;

r_device:
        class_destroy(dev_class);
r_class:
        unregister_chrdev_region(dev,1);
        cdev_del(&etx_cdev);
        return -1;
}

/* Module Exit Function */
static void __exit thre_driver_exit(void)
{
    kthread_stop(etx_threads);
    device_destroy(dev_class,dev);
    class_destroy(dev_class);
    cdev_del(&etx_cdev);
    unregister_chrdev_region(dev,1);
    pr_info("Kernel Thread Device Driver Remove ... Done !!\n");
}

/* Module init and exit */
module_init(thre_driver_init);
module_exit(thre_driver_exit);

/* Module Description */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Gladson");
MODULE_DESCRIPTION("KERNEL THREAD DEVICE DRIVER");
MODULE_VERSION("1.10");