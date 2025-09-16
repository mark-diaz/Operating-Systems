## Operating Systems Labs
The following are labs for the UCLA course CS111: Operating Systems Principles. They are all written in C and include kernel modules, interprocess communication, scheduling algorithms, and a file system for the Linux OS.

### Lab 0: A Kernel Seedling
---
This program is a kernel module for the Linux Operating System. It displays the number of processes running on a given machine.

### Lab 1: Pipe Up
---
This is a program to implement the pipe operator in Unix-based operating systems written in C. It creates a child process for each program and utilizes a pipe to transfer standard output from one program into the next.

### Lab 2: You Spin Me Round Robin
---
This program is an implementation of the round robin scheduling algorithm written in C. It simulates processes running on a CPU by storing a list of incoming processes including their: Process ID, Arrival Time, and Burst Time. 

### Lab 3: Hash Hash Hash
---
This is a implementation of a thread safe hash table written in C. It utilizes mutexes to provide mutual exclusion to eliminate race conditions. 

### Lab 4: Hey! I'm Filing Here
---
This in an implementation of the Ext2 (second extended file system) for the Linux Kernel written in C. It organizes data into block groups, which contain inodes and data blocks. Inodes store metadata about files such as permissions, timestamps, and pointers to the data blocks that hold the file's contents.
