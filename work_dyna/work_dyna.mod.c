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
	{ 0x122c3a7e, "_printk" },
	{ 0xbcab6ee6, "sscanf" },
	{ 0x3c3ff9fd, "sprintf" },
	{ 0x6228c21f, "smp_call_function_single" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0xed44a979, "cdev_init" },
	{ 0x411ffbe0, "cdev_add" },
	{ 0xb0c6c225, "class_create" },
	{ 0xfeae87a5, "device_create" },
	{ 0x61ef160f, "kernel_kobj" },
	{ 0x8f9f4ed6, "kobject_create_and_add" },
	{ 0x2618d724, "sysfs_create_file_ns" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0x17e56a8c, "cdev_del" },
	{ 0x15b3ce7d, "class_destroy" },
	{ 0x4e7480ee, "kobject_put" },
	{ 0x42692db9, "sysfs_remove_file_ns" },
	{ 0x69a20af4, "device_destroy" },
	{ 0x2d3385d3, "system_wq" },
	{ 0xc5b6f236, "queue_work_on" },
	{ 0x6f6ab014, "module_layout" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "1D9F7B96401DC34F1EE288C");
