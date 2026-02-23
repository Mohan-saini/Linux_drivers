#include <linux/module.h>
#include <linux/init.h>
#include <linux/ioctl.h>

#include "ioctl_header.h"

static int major;

int open(struct inode *inode, struct file *f)
{
    pr_info("char dev opened with major: %d, minor: %d\n",imajor(inode),iminor(inode));
    return 0;
}

int release(struct inode *inode, struct file *f)
{
    pr_info("released the device\n");
    return 0;
}
/* Global Variable for read/write example */
int32_t answer = 42;

long my_ioctl(struct file *fp, unsigned int cmd, unsigned long arg)
{
    struct my_struct test;
    switch(cmd) {
        case WR_VALUE:
            if (copy_from_user(&answer, (int32_t *)arg, sizeof(answer)))
                pr_err("err in ioctl write cmd\n");
            else
                pr_info("ioctl write updated answer to: %d\n",answer);
            break;
        case RD_VALUE:
            if (copy_to_user((int32_t *)arg, &answer, sizeof(answer)))
                pr_err("err in ioctl read cmd\n");
            else
                pr_info("ioctl read successful, sent answer value\n");
            break;
        case GREETER:
            if (copy_from_user(&test, (struct mystruct *)arg, sizeof(test)))
                pr_err("err in GREETER ioctl command\n");
            else
                pr_info("ioctl cmd: %d greets to %s\n",test.repeat, test.name);
            break;
    }
    return 0;
}

static struct file_operations fops = {
    .open = open,
    .release = release,
    .unlocked_ioctl = my_ioctl,
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
