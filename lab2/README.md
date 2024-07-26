# You Spin Me Round Robin

This program is an implementation of the round robin scheduling algorithm written in C. It simulates processes running on a CPU by storing a list of incoming processes including their: Process ID, Arrival Time, and Burst Time. 

## Implementation
In order to retrieve the process data from `process.txt` it parses through it and stores it in an array of process structs. This program utilizes a doubly linked list as a queue to store processes waiting to be executed. 

## Building
To build the executable, run the command: `make`

## Running
To run the program, run the command:
Note: Replace `quantum_length` with an integer you'd like to simulate the round robin scheduling algorithm with.

```shell
./rr processes.txt <quantum_length>
```
**Example**:
```shell
./rr processes.txt 3
```

**Results and Output**:
```shell
Average waiting time: 7.00
Average response time: 2.75
```

## Cleaning up
To clean and remove the executables run the command: `make clean`
