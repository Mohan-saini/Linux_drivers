#include <linux/module.h>
#include <linux/init.h>
#include <linux/kthread.h>
#include <linux/sched.h>
#include <linux/delay.h>
#include <linux/wait.h>
#include <linux/jiffies.h>

#define MYMAJOR 64

static struct task_struct *kthread_1;
static struct task_struct *kthread_2;

static int t1=1, t2=2;
/* watch variable to monitor with the wait queues*/
static long int watch_var = 0;

DECLARE_WAIT_QUEUE_HEAD(wq1); // first waitqueue, static one

static wait_queue_head_t wq2; // second wait queue, dynamic one

int thread_fun(void *arg)
{
    int num = *(int *)arg;

    switch(num) {
        case 1:
            wait_event(wq1, (watch_var == 11) || kthread_should_stop());
            if (kthread_should_stop())
                return 0;
            pr_info("waitqueue- watch var is now 11\n");
            break;
        case 2:
            while(wait_event_timeout(wq2, watch_var==22, msecs_to_jiffies(5000)) == 0)
                pr_info("waitqueue- watch_var is now 22\n");
            break;
        default:
            break;
    }

    pr_info("waitqueue- Thread monitoring wq %d finished\n",num);
    return 0;
}

static ssize_t my_write(struct file *File, const char *user_buffer, size_t count, loff_t *offset)
{
    int to_copy, not_copied, delta;
    char buffer[32];
    memset(buffer, 0, sizeof(buffer));

    to_copy = min(count,sizeof(buffer));
    not_copied = copy_from_user(buffer, user_buffer, to_copy);

    delta = to_copy - not_copied;
    if (kstrtol(buffer, 10, &watch_var))
        pr_err("waitqueue- error in converting input\n");
    pr_info("waitqueue- watch var now: %ld\n",watch_var);
    wake_up(&wq1);
    wake_up(&wq2);
    return delta;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .write = my_write,
};

static int __init entryModule(void)
{
    pr_info("waitqueue- Init threads\n");
    init_waitqueue_head(&wq2);

    if(register_chrdev(MYMAJOR, "my_dev_nr", &fops)){
        pr_err("couldn't register device file\n");
        return -1;
    }
    pr_info("device nr registered- my_dev_nr\n");

    kthread_1 = kthread_create(thread_fun, &t1, "kthread_1");
    if (kthread_1 != NULL) {
        wake_up_process(kthread_1);
        pr_info("kthread_1 is up and running now\n");
    }else {
        pr_info("kthread1 could not be created\n");
        unregister_chrdev(MYMAJOR, "my_dev_nr");
        kthread_stop(kthread_1);
        return -1;
    }

    kthread_2 = kthread_create(thread_fun, &t2, "kthread_2");
    if (kthread_2 != NULL) {
        wake_up_process(kthread_2);
        pr_info("kthread_2 is up and running now\n");
    }else {
        pr_info("kthread2 could not be created\n");
        unregister_chrdev(MYMAJOR, "my_dev_nr");
        kthread_stop(kthread_2);
        watch_var = 11;
        wake_up(&wq1);
        msleep(10);
        return -1;
    }

    return 0;
}

static void __exit exitModule(void)
{
    if(kthread_1)kthread_stop(kthread_1);
    if(kthread_2)kthread_stop(kthread_2);
    watch_var = 11;
    wake_up(&wq1);
    msleep(10);
    watch_var = 22;
    wake_up(&wq2);
    msleep(10);
    printk("bye linux\n");
    unregister_chrdev(MYMAJOR, "my_dev_nr");
}

module_init(entryModule);
module_exit(exitModule);

MODULE_LICENSE("GPL");
