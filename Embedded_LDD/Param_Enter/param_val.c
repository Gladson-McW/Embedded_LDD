/* Getting parameters for Driver Operation */

/* Include Header Files */

#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/moduleparam.h> /*<< Getting Module Inputs through this Header File Inclusion >>*/

int value_E, array_E[6];
char *name_E;
int cb_val=0;

module_param(value_E, int, S_IRUSR|S_IWUSR); // Integer Value Read and Write Value
module_param(name_E, charp, S_IRUSR|S_IWUSR); //character Read & Write
module_param_array(array_E, int, NULL, S_IRUSR|S_IWUSR); //Arry Value Read and Write

/* Callback function to display changed value*/
int notify_param(const char *val, const struct kernel_param *kp)
{
		int resue = param_set_int(val,kp); //set up the initial parameter value given
		if(resue==0){
			    printk(KERN_INFO "Call Back Function is Called");
			    printk(KERN_INFO "New value of cb_val is %d\n", cb_val);
			    return 0;
		}
		return -1;
}
/* kernel_param_ops structure in order to get formatted input to kernel system*/
const struct kernel_param_ops my_param_ops = 
{
		.set = &notify_param,
		.get = &param_get_int,
};

/* Module_param_cb to get all array inputs to the kernel as intended*/
module_param_cb(cb_val, &my_param_ops, &cb_val, S_IRUGO|S_IWUSR);

/* Module Initialization and parameter printing*/
static int __init param_initi(void)
{
		int i;
		printk(KERN_INFO "Value_E = %d \n", value_E);
		printk(KERN_INFO "cb_val = %d \n", cb_val);
		printk(KERN_INFO "Name = %s \n", name_E);
		for(i=0;i<(sizeof array_E/sizeof(int));i++)
		{
				printk(KERN_INFO "Array Value[%d] = %d\n", i, array_E[i]);
		}
		printk(KERN_INFO "Kernel Module Inserted Successfully...\n");
		return 0;
}


/* Module Exit*/
static void __exit param_exito(void)
{
		printk(KERN_INFO "Kernel Module Removed Successfully\n");
}

/* Module Initialization and Exit */
module_init(param_initi);
module_exit(param_exito);


/* License Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Gladson");
MODULE_DESCRIPTION("Parameter Value Given to Driver Kernel Module");
MODULE_VERSION("1.0");
