#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <unistd.h>

int main(int arg_count, char *arg_list[]) {
    // Check for command line arguments
    if (arg_count < 2) {
        // Print usage message if no command is given
        fprintf(stderr, "Usage: %s <command>\n", arg_list[0]);
        // Return with error status
        return 1;
    }
    // Declare variables to store process ID and exit status
    struct timeval begin, finish;
    pid_t process_id;
    int exit_status;
    // Get start time
    gettimeofday(&begin, NULL);

    // Fork a new process
    process_id = fork();

    // Check if fork was successful

    if (process_id == 0) {
        // In child process
        execvp(arg_list[1], &arg_list[1]);
        // Print error message if execvp fails
        perror("Error executing command");
        exit(EXIT_FAILURE);

    } else if (process_id > 0) {
        // In parent process
        waitpid(process_id, &exit_status, 0);
        gettimeofday(&finish, NULL);
        // Calculate elapsed time in seconds
        double duration = (finish.tv_sec - begin.tv_sec) + (finish.tv_usec - begin.tv_usec) / 1000000.0;
        printf("Elapsed time: %f seconds\n", duration);
    } else {
        // Print error message if fork fails
        perror("Fork failed");
        return 1;
    }
    return 0;
}
