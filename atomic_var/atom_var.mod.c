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
	{ 0xb3f7646e, "kthread_should_stop" },
	{ 0xa65c6def, "alt_cb_patch_nops" },
	{ 0xf9a482f9, "msleep" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0xed44a979, "cdev_init" },
	{ 0x411ffbe0, "cdev_add" },
	{ 0xb0c6c225, "class_create" },
	{ 0xfeae87a5, "device_create" },
	{ 0xa55e7f13, "kthread_create_on_node" },
	{ 0xd49d0373, "wake_up_process" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0x17e56a8c, "cdev_del" },
	{ 0x15b3ce7d, "class_destroy" },
	{ 0xb503507c, "kthread_stop" },
	{ 0x69a20af4, "device_destroy" },
	{ 0x6f6ab014, "module_layout" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "4896ECE620533F98DB15F1D");
