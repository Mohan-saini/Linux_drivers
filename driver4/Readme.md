This driver implements basic char device node with read/write function

[Test]:
1). make all
2). tmux, then switch to other tmux window using ctrl+B and then shift+%
3). dmesg -W in other observer tmux window
4). sudo insmod driver2.ko
5). sudo mknod /dev/chrdev c [major number that is shown in logs in other terminal] [minor number, used 0 here]
6). echo "Hello World!" | sudo tee /chrdev       ==> will write hello world to kernel buffer
7). sudo cat /dev/chrdev    ==> should print written buffer.
