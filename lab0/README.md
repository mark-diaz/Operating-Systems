# A Kernel Seedling
This program is a kernel module for the Linux Operating System. It displays the number of processes running on a given machine.
## Building
To build the program you first have to run the `make` command in the same directory as the make file. You then need to add the kernel module to the kernel.

**Commands for building binary and adding kernel module**:
```shell
make
sudo insmod proc_count.ko
```
## Running
To run the kernel module use the command `cat` which should show the number of processes running. Use the command:
```shell
cat /proc/count
```
**Results**:\
79
## Cleaning Up
To clean up the binary we created we can run the command `make clean`. To remove the kernel module from the kernel we can use the command `sudo rmmod proc_count.ko`

**Commands for cleaning the built binary and removing the kernel module**:
```shell
make clean
sudo rmmod proc_count.ko
```
## Testing
We can test our program using a Python Script that makes checks the output is correctly formatted, overall correctness, and efficiency.

**To run the script use this command in your shell**:
```python
python -m unittest
```
**As successful test run will output:**
```
...
---------------------
Ran 3 tests in 24.699s

OK
```
**Kernel Release Version** (obtained by using the command):
```shell
uname -r -s -v
```
Linux 5.14.8-arch-1 #1 SMP PREEMPT Sun, 26 Sep 2021 19:36:15 +0000