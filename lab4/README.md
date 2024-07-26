# Hey! I'm Filing Here

This in an implementation of the Ext2 (second extended file system) for the Linux Kernel written in C. It organizes data into block groups, which contain inodes and data blocks. Inodes store metadata about files such as permissions, timestamps, and pointers to the data blocks that hold the file's contents.

## Building

To build executable run the `make` command. 

## Running

To run the executable and create the `.img` file run the command: `./ext2-create`.

To verify the file system is correct run the command: `dumpe2fs cs111-base.img`.

To mount the file system run the following shell commands:
```shell
mkdir mnt
sudo mount -o loop cs111 -base.img mnt # mount your filesystem 
```

## Cleaning up
To clean the executables run the `make clean` command.

Also run the following shell commands:
```shell
sudo umount mnt # unmount the filesystem
rmdir mnt # delete the directory used for mounting 
```
