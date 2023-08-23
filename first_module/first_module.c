#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

// https://www.kernel.org/doc/html/latest/process/license-rules.html
MODULE_LICENSE("Dual MIT/GPL");

static int __init hello_init(void) {
    pr_info(KBUILD_MODNAME ": module loaded\n");
    return 0;
}

static void __exit hello_exit(void) {
    pr_info(KBUILD_MODNAME ": unloading module\n");
}

module_init(hello_init);
module_exit(hello_exit);