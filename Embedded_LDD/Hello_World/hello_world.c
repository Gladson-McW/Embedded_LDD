/* This is the basic demo module generation of linux device driver module */

/* Add the Kernel Header Files here */

#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>

static int __init hello_init(void)
{
	printk(KERN_INFO "Hello Everyone to Hello World Module\n");
	printk(KERN_INFO "A Basic Linux Device Driver Module\n");
	printk(KERN_INFO "Kernel Module Inserted Successfully...\n");
	return 0;
}

static void  __exit hello_exit(void)
{
	printk(KERN_INFO "Kernel Module Removed Successfully...\n");
}

/* INITIALIZATION OF MODULES */
module_init(hello_init);
module_exit(hello_exit);


/* MODULE INFORMATION SECTION */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Gladson");
MODULE_DESCRIPTION("Hello World Linux Device Driver Module");
MODULE_VERSION("1:1:0");
