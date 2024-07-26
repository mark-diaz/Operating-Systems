## UID: 005934321

## Pipe Up

This is a program to implement the pipe operator in Unix-based operating systems written in C. It creates a child process for each program and utilizes a pipe to transfer standard output from one program into the next.

## Building

To build the program run `make` in this directory. This should create the executable `pipe`.

## Running

To run the command type: `./pipe` followed by command line programs.

Example:
`$ ./pipe ls wc`

**Output**:

6       6      51

Note: this is equivalent to `ls | wc`


## Cleaning up

To clean all binary files run the command `make clean`. 
