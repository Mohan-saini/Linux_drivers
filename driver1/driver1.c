#include <linux/module.h>
#include <linux/init.h>

static int __init entryModule(void)
{
    printk("hello linux\n");
    return 0;
}

static void __exit exitModule(void)
{
    printk("bye linux\n");
}

module_init(entryModule);
module_exit(exitModule);

MODULE_LICENSE("GPL");
