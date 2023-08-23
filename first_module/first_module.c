// special linux kernel headers (standard libc is not available in the kernel)
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

// specify license of the module
// see https://www.kernel.org/doc/html/latest/process/license-rules.html
MODULE_LICENSE("Dual MIT/GPL");

// this function is executed when the module is loaded
static int __init hello_init(void) {
    // pr_info prints an information message to the syslog (read it using `dmesg`)
    // KBUILD_MODNAME is the name of the module
    pr_info(KBUILD_MODNAME ": module loaded\n");
    return 0;
}

// this function is executed before the module is unloaded
static void __exit hello_exit(void) {
    pr_info(KBUILD_MODNAME ": unloading module\n");
}

module_init(hello_init);
module_exit(hello_exit);