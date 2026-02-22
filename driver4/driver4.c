#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>

static int major;
static char buffer[64];

ssize_t read(struct file *file, char __user *u_buf, size_t len, loff_t *offset)
{
    int data_to_copy = ((*offset + len) < sizeof(buffer)) ? len : (sizeof(buffer) - *offset);
    pr_info("driver will send %d bytes of data\n",data_to_copy);
    if (*offset >= sizeof(buffer)) {
        pr_warn("offset already at end of kernel buffer\n");
        return 0;
    } 
    int not_copied = copy_to_user(u_buf, &buffer[*offset], data_to_copy);
    int delta = data_to_copy - not_copied;
    if (delta) {
        pr_warn("driver could only copy %d bytes\n",delta);
    }
    *offset += delta;
    return delta;
}

ssize_t write(struct file *file, const char __user *u_buf, size_t len, loff_t *offset)
{
    int data_to_copy = ((*offset + len) < sizeof(buffer)) ? len : (sizeof(buffer) - *offset);
    pr_info("driver will receive %d bytes of data\n",data_to_copy);
    if (*offset >= sizeof(buffer)) {
        pr_warn("offset already at end of kernel buffer\n");
        return 0;
    }
    int not_copied = copy_from_user(&buffer[*offset], u_buf, data_to_copy);
    int delta = data_to_copy - not_copied;
    if (delta) {
        pr_warn("driver could only copy %d bytes\n",delta);
    }
    *offset += delta;
    return delta;
}

static struct file_operations fops = {
    .read = read,
    .write = write,
};

static int __init entryModule(void)
{
    major = register_chrdev(0, "/dev/chrdev",&fops);
    if (major < 0) {
        pr_err("failed to allocate char device\n");
        return major;
    }
    pr_info("allocated char dev with major num: %d\n",major);
    return 0;
}

static void __exit exitModule(void)
{
    unregister_chrdev(major, "/dev/chrdev");
    pr_info("successfully unregistered char device\n");
}

module_init(entryModule);
module_exit(exitModule);

MODULE_LICENSE("GPL");
