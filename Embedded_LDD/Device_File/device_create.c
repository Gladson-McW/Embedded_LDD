/* Create a Device File for Character Driver (Manually)*/

/* Header File Inclusion */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>

dev_t dev = 0;

/*
 * Module Init Function
 */
 
static int __init device_inite(void)
{
		if((alloc_chrdev_region(&dev, 0, 1, "Device_Reg"))<0) {
		//	printk(KERN_ERROR "Cannot Allocate Major Number for device\n");
			pr_err("Cannot Allocate Major Number\n");
			return -1;
		}
		pr_info("Kernel Module Inserted Successfully ...\n");
		return 0;
		
}

/*
 * Module Exit Function
 */

static void __exit device_exite(void)
{
		unregister_chrdev_region(dev,1);
		pr_info("Kernel Module has been Removed Successfully");
}
/* Module Initialization and Exit Part */

module_init(device_inite);
module_exit(device_exite);

/* Module Information Section */

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Gladson");
MODULE_DESCRIPTION("Linux Driver Manually Creating a Device File");
MODULE_VERSION("1.1");


