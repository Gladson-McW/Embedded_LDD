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
	{ 0xecfb8223, "__spi_register_driver" },
	{ 0x900ad535, "misc_deregister" },
	{ 0xdcb764ad, "memset" },
	{ 0x7696f8c7, "__list_add_valid_or_report" },
	{ 0x84f63904, "__sanitizer_cov_trace_const_cmp1" },
	{ 0x12bed49, "__asan_store8_noabort" },
	{ 0x7a60c2f7, "spi_sync" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0xb325d817, "misc_register" },
	{ 0xe41c4315, "__sanitizer_cov_trace_const_cmp4" },
	{ 0xa3d55dad, "driver_unregister" },
	{ 0x656e4a6e, "snprintf" },
	{ 0x619cb7dd, "simple_read_from_buffer" },
	{ 0x98ec031b, "__asan_unregister_globals" },
	{ 0xf5869226, "__asan_register_globals" },
	{ 0x47560d3, "module_layout" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "9659A0B5BBE8156C82D4BBC");
