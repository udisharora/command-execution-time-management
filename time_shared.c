#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <unistd.h>

int main(int arg_count, char *arg_list[]) {
    if (arg_count < 2) {
        fprintf(stderr, "Usage: %s <command>\n", arg_list[0]);
        return 1;
    }

    pid_t process_id;
    int exit_status;
    key_t shm_key = 5678;
    int shm_id;
    struct timeval *time_segment;

    shm_id = shmget(shm_key, sizeof(struct timeval), IPC_CREAT | 0666);
    if (shm_id < 0) {
        perror("Shared memory creation failed");
        return 1;
    }

    time_segment = (struct timeval *)shmat(shm_id, NULL, 0);
    if (time_segment == (struct timeval *)-1) {
        perror("Shared memory attachment failed");
        return 1;
    }

    process_id = fork();

    if (process_id == 0) {
        gettimeofday(time_segment, NULL);
        shmdt((void *)time_segment);
        execvp(arg_list[1], &arg_list[1]);
        perror("Error executing command");
        exit(EXIT_FAILURE);
    } else if (process_id > 0) {
        waitpid(process_id, &exit_status, 0);

        struct timeval finish_time;
        gettimeofday(&finish_time, NULL);

        double duration = (finish_time.tv_sec - time_segment->tv_sec) + (finish_time.tv_usec - time_segment->tv_usec) / 1000000.0;
        printf("Elapsed time (shared memory): %f seconds\n", duration);

        shmdt((void *)time_segment);
        shmctl(shm_id, IPC_RMID, NULL);
    } else {
        perror("Fork failed");
        return 1;
    }

    return 0;
}
