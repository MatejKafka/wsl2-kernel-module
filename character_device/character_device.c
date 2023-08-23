/**
 * Example Linux kernel module, which creates a device file at `/dev/buffer`,
 * storing 128 bytes of data and allowing userspace applications to operate
 * on the buffer using file operations.
 */

#include <linux/device.h>
#include <linux/module.h>
#include <linux/uaccess.h>
#include <linux/printk.h>
#include <linux/fs.h>
#include <asm/errno.h>

MODULE_LICENSE("Dual MIT/GPL");

// /dev/buffer
#define DEVICE_NAME "buffer"

#define BUFFER_SIZE (size_t)128
static char g_buffer[BUFFER_SIZE] = {0};


static int device_open(struct inode* inode, struct file* file) {
    // increment module ref count
    try_module_get(THIS_MODULE);
    return 0;
}

static int device_release(struct inode* inode, struct file* file) {
    // decrement module ref count
    module_put(THIS_MODULE);
    return 0;
}

static ssize_t device_read(struct file* file, char __user* user_buffer, size_t length, loff_t* offset_ptr) {
    // validate the offset the user wants to read from
    loff_t offset = *offset_ptr;
    if (offset > BUFFER_SIZE) {
        return -EINVAL;
    }

    // ensure the user cannot read outside the bounds
    length = min(length, BUFFER_SIZE - (size_t)offset);
    // copy data to the user buffer (this also validates that the user has access to the memory at `*user_buffer`)
    if (copy_to_user(user_buffer, g_buffer + offset, length)) {
        return -EFAULT; // copy failed
    }

    // update file position
    *offset_ptr += length;
    // return the read size to the user
    return length;
}

static ssize_t device_write(struct file* file, const char __user* user_buffer, size_t length, loff_t* offset_ptr) {
    // validate the offset the user wants to write to
    loff_t offset = *offset_ptr;
    if (offset > BUFFER_SIZE) {
        return -EINVAL;
    }

    // ensure the user cannot read outside the bounds
    length = min(length, BUFFER_SIZE - (size_t)offset);
    // copy data from the user buffer (this also validates that the user has access to the memory at `*user_buffer`)
    if (copy_from_user(g_buffer + offset, user_buffer, length)) {
        return -EFAULT; // copy failed
    }

    // update file position
    *offset_ptr += length;
    // return the write size to the user
    return length;
}

// this struct declares which operations the device file supports
static struct file_operations fops = {
    .read = device_read,
    .write = device_write,
    .open = device_open,
    .release = device_release,
};


static struct class *cls;
static int major_dev_num;

static int __init chardev_init(void) {
    // register the character device
    int ret_val = register_chrdev(0, DEVICE_NAME, &fops);
    if (ret_val < 0) {
        pr_alert(KBUILD_MODNAME ": failed to register the character device: %d\n", ret_val);
        return ret_val;
    }

    major_dev_num = ret_val;

    // create the /dev/... file
    cls = class_create(THIS_MODULE, DEVICE_NAME);
    device_create(cls, NULL, MKDEV(major_dev_num, 0), NULL, DEVICE_NAME);

    pr_info(KBUILD_MODNAME ": device created at /dev/%s\n", DEVICE_NAME);

    return 0;
}

/* Cleanup - unregister the appropriate file from /proc */
static void __exit chardev_exit(void) {
    device_destroy(cls, MKDEV(major_dev_num, 0));
    class_destroy(cls);
    unregister_chrdev(major_dev_num, DEVICE_NAME);
}

module_init(chardev_init);
module_exit(chardev_exit);
