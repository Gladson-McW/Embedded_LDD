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
	{ 0xdde25e1, "class_create" },
	{ 0xd4ae550c, "device_create" },
	{ 0xfde7dc9b, "cdev_init" },
	{ 0x282d49c2, "cdev_add" },
	{ 0xc477136b, "i2c_get_adapter" },
	{ 0xc1e658bb, "i2c_new_client_device" },
	{ 0xacad428b, "i2c_register_driver" },
	{ 0x41790969, "i2c_put_adapter" },
	{ 0x71a55cd9, "i2c_smbus_read_byte_data" },
	{ 0xba5b1a73, "i2c_smbus_read_word_data" },
	{ 0x54027b28, "i2c_smbus_write_byte_data" },
	{ 0xcf435ce7, "device_destroy" },
	{ 0x378b3561, "class_destroy" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0xea59d69, "i2c_unregister_device" },
	{ 0x873a069b, "i2c_del_driver" },
	{ 0x6b8f7feb, "cdev_del" },
	{ 0xeb233a45, "__kmalloc" },
	{ 0x37a0cba, "kfree" },
	{ 0x12a4e128, "__arch_copy_from_user" },
	{ 0xdcb764ad, "memset" },
	{ 0x656e4a6e, "snprintf" },
	{ 0x6cbbfc54, "__arch_copy_to_user" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0xcc1bde15, "param_ops_int" },
	{ 0x773354b7, "module_layout" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "44F304F710BCAAE3DAEDF7C");
