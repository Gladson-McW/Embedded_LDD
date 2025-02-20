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
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0xfde7dc9b, "cdev_init" },
	{ 0x282d49c2, "cdev_add" },
	{ 0xdde25e1, "class_create" },
	{ 0xd4ae550c, "device_create" },
	{ 0x2bd5b16e, "kmalloc_caches" },
	{ 0x4fede42a, "kmalloc_trace" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0x378b3561, "class_destroy" },
	{ 0x37a0cba, "kfree" },
	{ 0xcf435ce7, "device_destroy" },
	{ 0x6b8f7feb, "cdev_del" },
	{ 0x12a4e128, "__arch_copy_from_user" },
	{ 0xdcb764ad, "memset" },
	{ 0x6cbbfc54, "__arch_copy_to_user" },
	{ 0x773354b7, "module_layout" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "C36CFFD22BDE852B26B97B1");
