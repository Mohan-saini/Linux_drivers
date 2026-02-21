This driver implements basic kernel module, which register the character device node, only have read function callback.

[Test]:
1). make all
2). tmux, then switch to other tmux window using ctrl+B and then shift+%
3). dmesg -W in other observer tmux window
4). sudo insmod driver2.ko
5). sudo mknod /dev/chrdev c [major number that is shown in logs in other terminal] [minor number, used 0 here]
6). cat /dev/chrdev
7). read function should be called in logs on other terminal window logs.

