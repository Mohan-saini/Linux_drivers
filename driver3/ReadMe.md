Created a kernel module with open and release functions.

Now, we can call this module from userspace program.

[Test]:
1). sudo insmod driver3.ko
2). check logs for major number
3). sudo mknod /dev/chrdev c [major] [minor]
4). ./test /dev/chrdev
5). Observe the kernel logs, with open and release functions being called.
