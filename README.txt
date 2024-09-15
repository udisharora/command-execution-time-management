Overview

This project consists of a program (time.c) that measures the time taken to run a command 
specified by the user via the command line. It implements three different methods to measure 
the time taken for the execution of a command:

Basic Method: Using fork() and exec() with gettimeofday() in the parent process.

Shared Memory Method: Sharing the start time between the child and parent processes using shared memory.

Message Passing Method: Sharing the start time using a pipe to pass data between the child and parent processes.

Code Structure

time.c: Contains the implementation of the basic method.
time_shm.c: Implements the shared memory approach.
time_pipe.c: Implements the message passing approach using pipes.
Makefile: Used to compile all the source files.
README.md: This file, describing the project and its different approaches.

HOW TO COMPILE:
make

HOW TO RUN:
./time <command>
ex: ./time ls


Comparisons in the different methods:

1. Method 1:

Simple implementation.
Measures the total time, which includes the overhead of process scheduling and context switches.
Output may not be highly accurate for commands that have very short execution times, 
as it includes system overhead.


2. Method 2:

Uses a shared memory segment to share the exact start time between the child and parent.
More precise than the basic method since the child passes the exact start time to the parent,
excluding the fork overhead.
Useful for more accurate time measurements in processes with shorter execution times.

3. Method 3:

Uses a pipe to communicate the start time from the child to the parent process.
Similar to shared memory in terms of precision but uses pipes for inter-process communication,
which may introduce a slight overhead.

Observations

The Basic Method often records slightly longer times due to the overhead of process scheduling and context 
switches.
Both the Shared Memory and Message Passing methods provide more accurate timing since they exclude the 
overhead involved in forking the process.
Message Passing adds a small amount of overhead compared to shared memory, as setting up a pipe and passing
data through it incurs additional system calls.


Conclusion

For commands with very short execution times, the Shared Memory or Message Passing methods are recommended
as they provide more accurate results by excluding overhead from process creation and scheduling. 
However, for longer-running processes, the difference between the three methods becomes negligible.
