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
  time_t arrival, start, finish;

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

      arrival = cmd_buffer[buf_tail].arrival_time;

      /* Send basic job metrics to perf_info for calculations */
      perf_info(arrival, start, finish);

      buf_tail++;
      count--;
      pthread_cond_signal(&cmd_buf_not_full);

      pthread_mutex_unlock(&cmd_queue_lock);
    }
    else
    { /* Child Process */
      /* Run the command scheduled in the queue */
      printf("here");
      execv(cmd_buffer[buf_tail].filename, NULL);
      return 0;
    }
  }
}