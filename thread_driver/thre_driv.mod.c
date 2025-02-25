#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/export-internal.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

#ifdef CONFIG_UNWINDER_ORC
#include <asm/orc_header.h>
ORC_HEADER;
#endif

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif



static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0xc6e2b94b, "filp_open" },
	{ 0x122c3a7e, "_printk" },
	{ 0xf9a482f9, "msleep" },
	{ 0xfff27cf0, "kernel_read" },
	{ 0xb3f7646e, "kthread_should_stop" },
	{ 0x9ec6ca96, "ktime_get_real_ts64" },
	{ 0xe4ff4742, "kernel_write" },
	{ 0xbc820d1d, "filp_close" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0xfdb0ec04, "gpiod_get_index" },
	{ 0x5172d28d, "gpiod_set_value" },
	{ 0x32802b29, "gpiod_put" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0xfde7dc9b, "cdev_init" },
	{ 0x282d49c2, "cdev_add" },
	{ 0xdde25e1, "class_create" },
	{ 0xd4ae550c, "device_create" },
	{ 0xef16c549, "kthread_create_on_node" },
	{ 0xe82ab024, "wake_up_process" },
	{ 0x378b3561, "class_destroy" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0x6b8f7feb, "cdev_del" },
	{ 0x98185062, "kthread_stop" },
	{ 0xcf435ce7, "device_destroy" },
	{ 0x773354b7, "module_layout" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "748404C302AAC3B3FAB60F2");
