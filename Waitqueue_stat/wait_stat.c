/* Implement the working of Waitqueue static Driver*/

/* include the header files */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/slab.h>        //kmalloc()
#include <linux/uaccess.h> //Implementing User Space Application copy_to/from_user
#include <linux/kthread.h> //Implementing kthread Operation
#include <linux/wait.h>
#include <linux/err.h> //IS_ERR()

//

uint32_t read_count=0;
static struct task_struct *wait_thread;

DECLARE_WAIT_QUEUE_HEAD(wait_queue_etx);

dev_t dev=0;
static struct class *dev_class;
static struct cdev etx_cdev;
int wait_queue_flag=0;

/* 
** Function Prototypes
*/

static int __init wait_st_init(void);
static void __exit wait_st_exit(void);

/***************Driver Functions*******************/
static int wait_st_open(struct inode *inode, struct file *file);
static int wait_st_release(struct inode *inode, struct file *file);
static ssize_t wait_st_read(struct file *filp, char __user *buf, size_t len, loff_t * off);
static ssize_t wait_st_write(struct file *filp, const char *buf, size_t len, loff_t * off);

/*
** File Operations Structure
*/

static struct file_operations fops = 
{
        .owner          = THIS_MODULE,
        .read           = wait_st_read,
        .write          = wait_st_write,
        .open           = wait_st_open,
        .release        = wait_st_release,
};

/*
** Thread function
*/

static int wait_function(void *unused)
{

        while(1) {
                pr_info("Waiting for the event......\n");
                wait_event_interruptible(wait_queue_etx, wait_queue_flag != 0);
                if(wait_queue_flag == 2) {
                    pr_info("Event Came From Exit Function...\n");
                    return 0;
                }
                pr_info("Event Came From Read Function - %d\n", ++read_count);
                wait_queue_flag=0;

        }
        do_exit(0);
        return 0;

}

/*
** This Function will be called when we open the Device file
*/

static int wait_st_open(struct inode *inode, struct file *file)
{
    pr_info("Wait_stat Device File Opened ....!!!!!\n");
    return 0;
}


/*
** This function will be called when we cloase the Device file
*/

static int wait_st_release(struct inode *inode, struct file *file)
{
    pr_info("Wait_stat Device File Closed...!!!\n");
    return 0;
}

/*
** This function will be called when we read the Device file
*/

static ssize_t wait_st_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
    pr_info("Read Function has been called in Driver..\n");
    wait_queue_flag = 1;
    wake_up_interruptible(&wait_queue_etx);
    return 0;
} 

static ssize_t wait_st_write(struct file *filp, const char *buf, size_t len, loff_t *off)
{
    pr_info("Write Function Enabled...\n");
    return len;
}
/*
** Module Initialization Function
*/



static int __init wait_st_init(void) 
{
    /* Allocating Major and Minor Number Dynamically*/

    if((alloc_chrdev_region(&dev, 0, 1,"etx_Dev_ws"))<0){
            pr_info("Cannot Allocate Major Number\n");
            return -1;
    }
    pr_info("Major = %d Minor = %d \n",MAJOR(dev), MINOR(dev));

    /* Creating cdev structure */
    cdev_init(&etx_cdev,&fops);
    etx_cdev.owner = THIS_MODULE;
    etx_cdev.ops = &fops;

    /* Adding the character device to the system */
    if((cdev_add(&etx_cdev,dev,1))<0){
        pr_info("Cannot add the device to the system\n");
        goto r_class;
    }

    /* Creating struct class */
    if(IS_ERR(dev_class=class_create("etx_class_ws")))
    {
        pr_info("Cannot create the struct class\n");
        goto r_class;
    }

    /* Creating device*/
    if(IS_ERR(device_create(dev_class,NULL,dev,NULL,"etx_device_ws")))
    {
        pr_info("Cannot create the Device 1 ... \n");
        goto r_device;
    }

    /* Create a Kernel thread with name 'mythread' */
    wait_thread = kthread_create(wait_function, NULL, "WaitThread");
    if (wait_thread) {
        pr_info("Thread Created Successfully.....\n");
        wake_up_process(wait_thread);
    }
    else
        pr_info("Thread creation failed\n");
    pr_info("wait_st Device Driver Insert Done !!!!\n");
    return 0;

r_device:
    class_destroy(dev_class);
r_class:
    unregister_chrdev_region(dev,1);
    return -1;
}

/*
** Module Exit Function 
*/

static void __exit wait_st_exit(void)
{
        wait_queue_flag = 2;
        wake_up_interruptible(&wait_queue_etx);
        device_destroy(dev_class,dev);
        class_destroy(dev_class);
        cdev_del(&etx_cdev);
        unregister_chrdev_region(dev,1);
        pr_info("wait_st Device Driver Remove ... Done!!!\n");
}

/* Module Initialization and Exit*/
module_init(wait_st_init);
module_exit(wait_st_exit);

/* Module Description and Info*/
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Gladson");
MODULE_VERSION("1.7");
MODULE_DESCRIPTION("WAIT QUEUE STATIC DEVICE DRIVER");
