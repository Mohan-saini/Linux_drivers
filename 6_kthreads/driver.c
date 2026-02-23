#include <linux/module.h>
#include <linux/init.h>
#include <linux/kthread.h>
#include <linux/sched.h>
#include <linux/delay.h>

static struct task_struct *kthread_1;
static struct task_struct *kthread_2;
static int t1=1, t2=2;

int thread_fun(void *arg)
{
    unsigned int i = 0;
    int num = *(int *)arg;

    while(!kthread_should_stop()){
        pr_info("kthread- Thread %d is executing, counter: %d\n",num,i);
        i++;
        msleep(1000);
    }

    pr_info("kthread- Thread %d finished\n",num);
    return 0;
}

static int __init entryModule(void)
{
    kthread_1 = kthread_create(thread_fun, &t1, "kthread_1");
    if (kthread_1 != NULL) {
        wake_up_process(kthread_1);
        pr_info("kthread_1 is up and running now\n");
    }else {
        pr_info("kthread1 could not be created\n");
        kthread_stop(kthread_1);
        return -1;
    }

    kthread_2 = kthread_create(thread_fun, &t2, "kthread_2");
    if (kthread_2 != NULL) {
        wake_up_process(kthread_2);
        pr_info("kthread_2 is up and running now\n");
    }else {
        pr_info("kthread2 could not be created\n");
        kthread_stop(kthread_2);
        return -1;
    }

    return 0;
}

static void __exit exitModule(void)
{
    kthread_stop(kthread_1);
    kthread_stop(kthread_2);
    printk("bye linux\n");
}

module_init(entryModule);
module_exit(exitModule);

MODULE_LICENSE("GPL");
