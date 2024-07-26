#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/queue.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

typedef uint32_t u32;
typedef int32_t i32;

struct process
{
  u32 pid;
  u32 arrival_time;
  u32 burst_time;
  
  TAILQ_ENTRY(process) pointers;

  /* Additional fields here */
  u32 remaining_time;
  u32 finish_time;
  u32 first_run_time;
  bool has_run;
  /* End of "Additional fields here" */
};

TAILQ_HEAD(process_list, process);

// gets int from data stream
u32 next_int(const char **data, const char *data_end) 
{
  u32 current = 0;
  bool started = false;
  while (*data != data_end)
  {
    char c = **data;
    
    // check if digit
    if (c < 0x30 || c > 0x39)
    {
      if (started)
      {
        return current;
      }
    }
    else
    {
      if (!started)
      {
        current = (c - 0x30); // extract integer
        started = true;
      }
      else
      {
        current *= 10;
        current += (c - 0x30);
      }
    }

    ++(*data);
  }

  printf("Reached end of file while looking for another integer\n");
  exit(EINVAL);
}

// gets int from command line: used for third argument
u32 next_int_from_c_str(const char *data)
{
  char c;
  u32 i = 0;
  u32 current = 0;
  bool started = false;
  while ((c = data[i++]))
  {
    if (c < 0x30 || c > 0x39)
    {
      exit(EINVAL);
    }
    if (!started)
    {
      current = (c - 0x30);
      started = true;
    }
    else
    {
      current *= 10;
      current += (c - 0x30);
    }
  }
  return current;
}

// opens file and iterates through processes in file
void init_processes(const char *path,
                    struct process **process_data,
                    u32 *process_size)
{
  // open file
  int fd = open(path, O_RDONLY);
  if (fd == -1)
  {
    int err = errno;
    perror("open");
    exit(err);
  }

  // get file size
  struct stat st;
  if (fstat(fd, &st) == -1)
  {
    int err = errno;
    perror("stat");
    exit(err);
  }

  u32 size = st.st_size;
  // maps file into memory using mmap()
  const char *data_start = mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);
  if (data_start == MAP_FAILED)
  {
    int err = errno;
    perror("mmap");
    exit(err);
  }

  // set start and end of file
  const char *data_end = data_start + size;
  const char *data = data_start;

  // get process size from first line of file
  *process_size = next_int(&data, data_end);

  // allocate enough memory for processes size 
  *process_data = calloc(sizeof(struct process), *process_size);
  if (*process_data == NULL)
  {
    int err = errno;
    perror("calloc");
    exit(err);
  }

  // iterate through processes giving each one correct member variables
  for (u32 i = 0; i < *process_size; ++i)
  {
    (*process_data)[i].pid = next_int(&data, data_end);
    (*process_data)[i].arrival_time = next_int(&data, data_end);
    (*process_data)[i].burst_time = next_int(&data, data_end);
  }

  // unmap memory and close file
  munmap((void *)data, size);
  close(fd);
}

// ./rr processes.txt 3
int main(int argc, char *argv[])
{
  if (argc != 3)
  {
    return EINVAL;
  }
  // data is an array of processes (reference to first index) 
  struct process *data;
  u32 size; // number of processes
  
  // create dynamic array of processes from txt file
  init_processes(argv[1], &data, &size);

  // quantum length from 3rd command line argument
  u32 quantum_length = next_int_from_c_str(argv[2]);

  struct process_list list;
  TAILQ_INIT(&list);

  u32 total_waiting_time = 0;
  u32 total_response_time = 0;

  /* Your code here */

  u32 time = 0;
  u32 run_time = 0;
  u32 processes_complete = 0;
  struct process* current_process_running; 

  // initialize additional fields in processes 
  for (u32 i = 0; i < size; i++){
      data[i].finish_time = 0;
      data[i].remaining_time = data[i].burst_time;
      data[i].first_run_time = 0;
      data[i].has_run = false;

    // add process that arrives at time = 0
    if (data[i].arrival_time == time) {
      TAILQ_INSERT_TAIL(&list, &data[i], pointers);
    }
  }
  
  // there are still processes to complete
  while(processes_complete != size){ 
    bool cpu_running = false;

    if (!TAILQ_EMPTY(&list)) {
      // run first process in queue
      current_process_running = TAILQ_FIRST(&list);
      TAILQ_REMOVE(&list, current_process_running, pointers);

      // record first time process is run
      if (!current_process_running->has_run) {
        current_process_running->has_run = true;
        current_process_running->first_run_time = time;
      }

      // process should run for time slice unless remaining time is smaller
      if (current_process_running->remaining_time > quantum_length) {
        run_time = quantum_length;
      }
      else {
        run_time = current_process_running->remaining_time;
      }
      current_process_running->remaining_time -= run_time;
      // check for new arrivals
      for (u32 i = 0; i < run_time; i++){
        time++;
        // loop through processes to check arrival times
        for (u32 j = 0; j < size; j++){
          // add new process if it arrives
          if (!data[j].has_run && data[j].arrival_time == time) {
            TAILQ_INSERT_TAIL(&list, &data[j], pointers);
          }
        }
      }

      // record time processed finished and incrememnt processes complete
      if (current_process_running->remaining_time == 0){
        processes_complete++;
        current_process_running->finish_time = time;
      }
      else {
        // if process is not done put it back in end of queue
        TAILQ_INSERT_TAIL(&list, current_process_running, pointers);

      }
      cpu_running = true;
    }
    
    // CPU is not running (no processes in queue)
    if (!cpu_running) {
      time++;
      // check for arriving processes
      for (u32 i = 0; i < size; i++){
        if (!data[i].has_run && data[i].arrival_time == time) {
          TAILQ_INSERT_TAIL(&list, &data[i], pointers);
        }
      }
    }
  }

  // Do calculations for waiting and response time
  for(u32 i = 0; i < size; i++){
    total_waiting_time += ((data[i].finish_time - data[i].arrival_time) - data[i].burst_time) ;
    total_response_time += (data[i].first_run_time - data[i].arrival_time);
  }

  /* End of "Your code here" */

  printf("Average waiting time: %.2f\n", (float)total_waiting_time / (float)size);
  printf("Average response time: %.2f\n", (float)total_response_time / (float)size);

  free(data);
  return 0;
}
