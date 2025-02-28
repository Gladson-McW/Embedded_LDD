#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/workqueue.h>
#include <linux/interrupt.h>
#include <linux/smp.h>

#define IRQ_NO 11

static dev_t dev;
static struct cdev etx_cdev;
static struct class *dev_class;
static struct kobject *kobj_ref;
static struct workqueue_struct *own_workqueue;
static int etx_value = 0;

struct my_list {
    struct list_head list;
    int data;
};
LIST_HEAD(Head_Node);

static void workqueue_fn(struct work_struct *work);
static DECLARE_WORK(work, workqueue_fn);

static ssize_t sysfs_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    return sprintf(buf, "%d", etx_value);
}
void interrupt_call(void* info);

void interrupt_call(void* info)
{
   workqueue_fn(NULL);
}
static ssize_t sysfs_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
    sscanf(buf, "%d", &etx_value);
    smp_call_function(interrupt_call, NULL, 1);
    return count;
}

static struct kobj_attribute etx_attr = __ATTR(etx_value, 0660, sysfs_show, sysfs_store);

static void workqueue_fn(struct work_struct *work)
{
    struct my_list *temp_node = kmalloc(sizeof(struct my_list), GFP_KERNEL);
    if (!temp_node)
        return;
    temp_node->data = etx_value;
    INIT_LIST_HEAD(&temp_node->list);
    list_add_tail(&temp_node->list, &Head_Node);
}

static int etx_open(struct inode *inode, struct file *file)
{
      printk(KERN_INFO "Device Has been Opened!\n");
      return 0;
}

static int etx_release(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "Device has been Closed!\n");
    return 0;
}

static ssize_t etx_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
    struct my_list *temp;
    int count = 0;
    list_for_each_entry(temp, &Head_Node, list) {
        printk(KERN_INFO "Node %d data = %d\n", count++, temp->data);
    }
    return 0;
}



static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = etx_open,
    .release = etx_release,
    .read = etx_read,
};

static int __init etx_driver_init(void)
{
    if (alloc_chrdev_region(&dev, 0, 1, "etx_Dev_link") < 0)
        return -1;
    cdev_init(&etx_cdev, &fops);
    if (cdev_add(&etx_cdev, dev, 1) < 0)
        goto r_class;
    if (IS_ERR(dev_class = class_create("etx_class_link")))
        goto r_class;
    if (IS_ERR(device_create(dev_class, NULL, dev, NULL, "etx_device_link")))
        goto r_device;
    kobj_ref = kobject_create_and_add("etx_sysfs_link", kernel_kobj);
    if (sysfs_create_file(kobj_ref, &etx_attr.attr))
        goto r_sysfs;
    own_workqueue = create_workqueue("own_wq");
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
    struct my_list *cursor, *temp;
    list_for_each_entry_safe(cursor, temp, &Head_Node, list) {
        list_del(&cursor->list);
        kfree(cursor);
    }
    destroy_workqueue(own_workqueue);
    kobject_put(kobj_ref);
    sysfs_remove_file(kernel_kobj, &etx_attr.attr);
    device_destroy(dev_class, dev);
    class_destroy(dev_class);
    cdev_del(&etx_cdev);
    unregister_chrdev_region(dev, 1);
}

/* Module Init and Exit */
module_init(etx_driver_init);
module_exit(etx_driver_exit);

/* Module Description and Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Gladson");
MODULE_DESCRIPTION("Sysfs and Character Driver with Workqueue and Linked List");

