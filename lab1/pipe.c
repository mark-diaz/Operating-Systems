#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

int main(int argc, char *argv[])
{	
	// read only pointer to argv
	const char* process1 = argv[1];

	// initial error checks
	if (argc <= 1){
		fprintf(stderr, "Arguments\n");
		exit(EINVAL);
	}
	// handle single program case
	else if (argc == 2){		
		if(execlp(process1, process1, NULL) == -1){
			perror("execlp");
			exit(errno);
		}
	}

	// set up file descriptor for initial pipe
	int fd[2];
	if (pipe(fd) == -1){
		perror("pipe");
		exit(errno);
	}

	// Create child 1
	pid_t child1_pid;
	child1_pid = fork();

	if(child1_pid == -1){
		perror("fork");
		exit(errno);
	}

	if(child1_pid == 0){
		// child process
		
		// point standard output to write end of pipe
		dup2(fd[1], STDOUT_FILENO);
		close(fd[0]);
		close(fd[1]);
		if(execlp(process1, process1, NULL) == -1){
			perror("execlp");
			exit(errno);
		}
		// Code here will never be executed

	} else{ 
		// parent process
		int status = 0;
		waitpid(child1_pid, &status, 0);
		if(!WIFEXITED(status) || WEXITSTATUS(status) != 0){ 
			exit(WEXITSTATUS(status));
		}

		// direct standard input to read end of pipe
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		close(fd[1]);

		int fdN[2];
		int prev_fd = -1;
		const char* processN;

		for(int i = 2; i < argc; i++){
			processN = argv[i];

			// create new pipe every command except for last one
			if(i < argc - 1) 
			{
				if (pipe(fdN) == -1){
					perror("pipe");
					exit(errno);
				}
			}

			// create child N
			pid_t childN_pid;
			childN_pid = fork();

			if(childN_pid == -1){
				perror("fork");
				exit(errno);
			}

			if (childN_pid == 0){
			// child N process
			
				// not first iteration
				if (prev_fd != -1){ 
					// direct standard input to the previous pipe's read end
					dup2(prev_fd, STDIN_FILENO);
					close(prev_fd);
				}
				
				// For every command except for last one
				if (i < argc - 1){
					// point standard output to write end of pipe
					dup2(fdN[1], STDOUT_FILENO);
					close(fdN[0]);
					close(fdN[1]);
				}
				
				if(execlp(processN, processN, NULL) == -1){
					perror("execlp");
					exit(errno);
				}
			}
			else {
				// back in parent
				if (prev_fd != -1){
					close(prev_fd);
				}

				if (i < argc - 1){
					// save the previous pipe's read end 
					prev_fd = fdN[0];
					close(fdN[1]);
				}
				
				int status = 0;
				waitpid(childN_pid, &status, 0);
				if(!WIFEXITED(status) || WEXITSTATUS(status) != 0){ 
					exit(WEXITSTATUS(status));
				}
			}
		}

	}
	return 0;
}