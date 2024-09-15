#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <unistd.h>

int main(int arg_count, char *arg_list[]) {
    if (arg_count < 2) {
        fprintf(stderr, "Usage: %s <command>\n", arg_list[0]);
        return 1;
    }

    pid_t process_id;
    int pipe_fds[2], exit_status;
    struct timeval time_start, time_end;

    if (pipe(pipe_fds) == -1) {
        perror("Pipe creation failed");
        return 1;
    }

    process_id = fork();

    if (process_id == 0) {
        close(pipe_fds[0]);
        gettimeofday(&time_start, NULL);
        write(pipe_fds[1], &time_start, sizeof(struct timeval));
        close(pipe_fds[1]);

        execvp(arg_list[1], &arg_list[1]);
        perror("Error executing command");
        exit(EXIT_FAILURE);
    } else if (process_id > 0) {
        close(pipe_fds[1]);
        waitpid(process_id, &exit_status, 0);
        read(pipe_fds[0], &time_start, sizeof(struct timeval));
        close(pipe_fds[0]);

        gettimeofday(&time_end, NULL);
        double duration = (time_end.tv_sec - time_start.tv_sec) + (time_end.tv_usec - time_start.tv_usec) / 1000000.0;
        printf("Elapsed time (pipe): %f seconds\n", duration);
    } else {
        perror("Fork failed");
        return 1;
    }

    return 0;
}
