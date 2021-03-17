/*
 * COMP 7500/06: Project 3 AUbatch
 * Michael Blakley
 * mzb0155@auburn.edu
 * Auburn University
 *
 * 
 * Compilation Instructions:
 * Compile at project root with $make all, then run $./aubatch
 * Individually compile with 
 * gcc dispatcher.c -o dispather
 * ./dispatcher
 *
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <string.h>
#include <time.h>
#include "../../include/aubatch.h"

extern pthread_mutex_t cmd_queue_lock;
extern pthread_cond_t cmd_buf_not_full;
extern pthread_cond_t cmd_buf_not_empty;
extern u_int buf_tail;
extern u_int count;
extern struct Job *cmd_buffer;

void *dispatcher(void *ptr)
{
  pid_t pid;
  time_t start, finish;

  while (1)
  {
    pthread_mutex_lock(&cmd_queue_lock);

    while (count == 0)
    {
      pthread_cond_wait(&cmd_buf_not_empty, &cmd_queue_lock);
    }

    /* Update status of job to running with queue locked */
    strcpy(cmd_buffer[buf_tail].status, "Running");

    /* Update Starting Time */
    start = time(NULL);
    printf("start: %lu\n", start);
    cmd_buffer[buf_tail].start_time = start;

    /* Create Fork */
    pid = fork();

    if (pid > 0)
    { /* Parent Process */
      /* unlock and wait for child process to terminate */
      pthread_mutex_unlock(&cmd_queue_lock);
      wait(NULL);

      /* job finished - acquire lock and update buffer and count */
      pthread_mutex_lock(&cmd_queue_lock);

      /* Log Finish Time */
      finish = time(NULL);
      cmd_buffer[buf_tail].finish_time = finish;

      /* Calc CPU_Time */
      printf("cpu_time: %ld\n", finish - start);
      cmd_buffer[buf_tail].cpu_time = finish - start;

      /* Calc Turnaround Time */
      printf("ta_time: %ld\n", finish - cmd_buffer[buf_tail].arrival_time);
      cmd_buffer[buf_tail].turn_around_time = finish - cmd_buffer[buf_tail].arrival_time;

      /* Calc Waiting Time */
      cmd_buffer[buf_tail].wait_time = cmd_buffer[buf_tail].turn_around_time - cmd_buffer[buf_tail].cpu_time;

      perf_info(&cmd_buffer[buf_tail]);

      buf_tail++;
      count--;
      pthread_cond_signal(&cmd_buf_not_full);

      pthread_mutex_unlock(&cmd_queue_lock);
    }
    else
    { /* Child Process */
      /* Run the command scheduled in the queue */
      execv(cmd_buffer[buf_tail].filename, NULL);
      return 0;
    }
  }
}