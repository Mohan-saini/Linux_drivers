#include <linux/module.h>
#include <linux/init.h>

int major;

int open(struct inode *inode, struct file *f)
{
    pr_info("char dev opened with major: %d, minor: %d\n",imajor(inode),iminor(inode));

    pr_info("flags of file: %X\n", f->f_flags);
    pr_info("f_mode of file: %X\n", f->f_mode);
    return 0;
}

int release(struct inode *inode, struct file *f)
{
    pr_info("released the device\n");
    return 0;
}

static struct file_operations fops = {
    .open = open,
    .release = release,
};

static int __init entryModule(void)
{
    major = register_chrdev(0,"/dev/chrdev", &fops);
    if (major < 0) {
        pr_err("unable to register char device: %d\n",major);
        return major;
    }
    pr_info("char device created with major number: %d\n",major);
    return 0;
}

static void __exit exitModule(void)
{
    pr_info("bye linux\n");
}

module_init(entryModule);
module_exit(exitModule);

MODULE_LICENSE("GPL");
