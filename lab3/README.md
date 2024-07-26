# Hash Hash Hash
This is a implementation of a thread safe hash table written in C. It utilizes mutexes to provide mutual exclusion to eliminate race conditions. 

## Building
To build the program run the `make` command in this directory.

## Running
To run the program type the command:
```shell
./hash-table-tester -t num_threads -s num_entries
```
Replace `num_threads` with the number of threads you would like to run this program with. Replace `num_entries` with the number of hash table entries you would like to test this program with.

## First Implementation
In the `hash_table_v1_add_entry` function, I added a single lock around the entire hash table. I used this lock in the `add_entry()` function as this is where race conditions occurred.

### Performance
Run:
`./hash-table-tester -t 4 -s 150000`

Results:
```
Generation: 103,957 usec
Hash table base: 3,272,776 usec
  - 0 missing
Hash table v1: 5,243,173 usec
  - 0 missing
Generation: 102,711 usec
Hash table base: 3,266,840 usec
  - 0 missing
Hash table v1: 5,279,275 usec
  - 0 missing
Generation: 103,925 usec
Hash table base: 3,263,908 usec
  - 0 missing
Hash table v1: 5,462,144 usec
  - 0 missing
```
Version 1 is a lot slower than the base version. As the threads compete for a single lock protecting the hash table. This added overhead of a single lock makes multi-threading worse than the base version creating a bottleneck. I used these locks in the `add_entry()` function as this is where race conditions occurred.

## Second Implementation
In the `hash_table_v2_add_entry` function, I added a lock for each entry of the has table. This is because race conditions occur when a hash table entries are added to the same bucket at the same time. Using a single lock around each bucket we can obtain faster performance and eliminate race conditions

### Performance
To run:
`./hash-table-tester -t 4 -s 150000`

Results:
```
Run 1:
Generation: 102,857 usec
Hash table base: 3,182,592 usec
  - 0 missing
Hash table v2: 990,051 usec
  - 0 missing
Run 2:
Generation: 104,432 usec
Hash table base: 3,307,914 usec
  - 0 missing
Hash table v2: 995,839 usec
  - 0 missing
Run 3:
Generation: 104,741 usec
Hash table base: 3,355,834 usec
  - 0 missing
Hash table v2: 1,022,666 usec
  - 0 missing
```

As we can see we achieved a three times speed up with four threads meeting the performance constraints. With locks around each bucket there is an increase in parallelism and eliminating the bottleneck. This also provides mutual exclusion for each bucket and eliminates race conditions.
## Cleaning up
To clean all binary files run the command `make clean`. 
