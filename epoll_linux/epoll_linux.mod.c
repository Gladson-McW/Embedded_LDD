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
	{ 0x88a009b6, "__sanitizer_cov_trace_pc" },
	{ 0x122c3a7e, "_printk" },
	{ 0xe914e41e, "strcpy" },
	{ 0x5aa09c0b, "__wake_up" },
	{ 0x4829a47e, "memcpy" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0xe41c4315, "__sanitizer_cov_trace_const_cmp4" },
	{ 0x21db8dd, "cdev_init" },
	{ 0x71eca315, "cdev_add" },
	{ 0xf3914493, "class_create" },
	{ 0x159385bc, "__sanitizer_cov_trace_const_cmp8" },
	{ 0x7e68d1f5, "device_create" },
	{ 0x7dc89edf, "kernel_kobj" },
	{ 0xfbc681e9, "__asan_load8_noabort" },
	{ 0x7587b518, "kobject_create_and_add" },
	{ 0xd59a641, "sysfs_create_file_ns" },
	{ 0x7e29b874, "kobject_put" },
	{ 0xefbb10b, "sysfs_remove_file_ns" },
	{ 0x93807586, "class_destroy" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0x93a2224d, "device_destroy" },
	{ 0xb3e6822b, "cdev_del" },
	{ 0x84f63904, "__sanitizer_cov_trace_const_cmp1" },
	{ 0x54b1fac6, "__ubsan_handle_load_invalid_value" },
	{ 0xb70789e, "__might_fault" },
	{ 0x414ed65b, "should_fail_usercopy" },
	{ 0xcccd8c24, "__asan_load4_noabort" },
	{ 0x4cb6d47e, "__sanitizer_cov_trace_cmp8" },
	{ 0x83645119, "__kasan_check_write" },
	{ 0x12a4e128, "__arch_copy_from_user" },
	{ 0xdcb764ad, "memset" },
	{ 0x87a21cb3, "__ubsan_handle_out_of_bounds" },
	{ 0xbb0367a1, "__asan_store1_noabort" },
	{ 0x98cf60b3, "strlen" },
	{ 0x8e9826ec, "__kasan_check_read" },
	{ 0x6cbbfc54, "__arch_copy_to_user" },
	{ 0x98ec031b, "__asan_unregister_globals" },
	{ 0xf5869226, "__asan_register_globals" },
	{ 0x47560d3, "module_layout" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "385F4C6891C901446332075");
