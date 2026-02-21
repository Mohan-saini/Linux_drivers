#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>

int major;

ssize_t read(struct file *file, char __user *u, size_t, loff_t *pos)
{
    printk("read function called for this device node\n");
    return 0;
}

static struct file_operations fops = {
    .read = read,
};

static int __init entryModule(void)
{
    major = register_chrdev(0, "/dev/chrdev",&fops);
    if (major < 0) {
        printk("failed to allocate char device\n");
        return major;
    }
    printk("allocated char dev with major num: %d\n",major);
    return 0;
}

static void __exit exitModule(void)
{
    unregister_chrdev(major, "/dev/chrdev");
    printk("successfully unregistered char device\n");
}

module_init(entryModule);
module_exit(exitModule);

MODULE_LICENSE("GPL");
