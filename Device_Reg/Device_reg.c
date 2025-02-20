/* Register a device and class Automatically */

/* Include Header Files */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/err.h>
#include <linux/device.h>

dev_t dev=0;
static struct class *dev_class;

/*
 * Module Init Function
 */
 
static int __init device_inita(void)
{
		if((alloc_chrdev_region(&dev,0,1,"device_name"))<0) {
			
			pr_err("Cannot Allocate Major Number for Deviice\n");
			return -1;
	     }
		dev_class = class_create("device_class");
		if(IS_ERR(dev_class)) {
			pr_err("Cannot create the struct class for device\n");
			goto r_class;
		}
		
		if(IS_ERR(device_create(dev_class,NULL,dev,NULL,"etx_device"))) {
			pr_err("Cannot Create the Device\n");
			goto r_device;
		}
		pr_info("Kernel Module Inserted Successfullyyyyyyyyyyyyyy...\n");
		return 0;
		
r_device:
		class_destroy(dev_class);
r_class:
		unregister_chrdev_region(dev,1);
		return -1;
}
/* Module Exit Function */
static void __exit device_exita(void)
{
		device_destroy(dev_class,dev);
		class_destroy(dev_class);
		unregister_chrdev_region(dev,1);
		pr_info("Kernel Module Removed Successfully...\n");
}
/* Module Init and Exit */
module_init(device_inita);
module_exit(device_exita);

/* Module Registration */

MODULE_LICENSE("GPL");
MODULE_AUTHOR("GLadson");
MODULE_DESCRIPTION("Automatically Creating a Device File");
MODULE_VERSION("1.2");

