#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
 .name = KBUILD_MODNAME,
 .init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
 .exit = cleanup_module,
#endif
 .arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0xac902c3f, "module_layout" },
	{ 0x36f8040, "class_destroy" },
	{ 0xc7034798, "device_destroy" },
	{ 0xbe2f28b, "cdev_del" },
	{ 0xfe990052, "gpio_free" },
	{ 0xf20dabd8, "free_irq" },
	{ 0x7485e15e, "unregister_chrdev_region" },
	{ 0x2072ee9b, "request_threaded_irq" },
	{ 0x11f447ce, "__gpio_to_irq" },
	{ 0x65d6d0f0, "gpio_direction_input" },
	{ 0x47229b5c, "gpio_request" },
	{ 0xc7f3a5f0, "device_create" },
	{ 0x7617bcba, "__class_create" },
	{ 0x60641d6a, "cdev_add" },
	{ 0x4db2466a, "cdev_init" },
	{ 0x29537c9e, "alloc_chrdev_region" },
	{ 0x4f68e5c9, "do_gettimeofday" },
	{ 0x6c8d5ae8, "__gpio_get_value" },
	{ 0x8e865d3c, "arm_delay_ops" },
	{ 0x432fd7f6, "__gpio_set_value" },
	{ 0xa8f59416, "gpio_direction_output" },
	{ 0x27e1a049, "printk" },
	{ 0x2e5810c6, "__aeabi_unwind_cpp_pr1" },
	{ 0xb1ad28e0, "__gnu_mcount_nc" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

