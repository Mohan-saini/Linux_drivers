LKM with ioctl function implemented.

[Test]:
1). sudo insmod driver3.ko
2). check logs for major number
3). sudo mknod /dev/chrdev c [major] [minor]
4). sudo ./test
5). Can see in test app logs, ioctl called successfully.
