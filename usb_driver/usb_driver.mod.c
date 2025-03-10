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
	{ 0x414750c4, "usb_register_driver" },
	{ 0x2dce11e7, "_dev_info" },
	{ 0xfbc681e9, "__asan_load8_noabort" },
	{ 0x3af089e2, "__asan_load2_noabort" },
	{ 0x41ee0b01, "__asan_load1_noabort" },
	{ 0x122c3a7e, "_printk" },
	{ 0xe41c4315, "__sanitizer_cov_trace_const_cmp4" },
	{ 0x9e93b930, "__sanitizer_cov_trace_cmp4" },
	{ 0x919f6aae, "usb_deregister" },
	{ 0x98ec031b, "__asan_unregister_globals" },
	{ 0xf5869226, "__asan_register_globals" },
	{ 0x47560d3, "module_layout" },
};

MODULE_INFO(depends, "");

MODULE_ALIAS("usb:v22D9p2764d*dc*dsc*dp*ic*isc*ip*in*");

MODULE_INFO(srcversion, "20CF8441188AC3533EAA20D");
