/*
 * COMP 7500/06: Project 3 AUbatch
 * Michael Blakley
 * mzb0155@auburn.edu
 * Auburn University
 * 
 * A large portion of this file was provided as sample code from
 * Dr. Xiao Qin - Dept. of CS & SE - Auburn University

 * How to Compile?
 * $make all
 * 
 * How to Run?
 * ./aubatch
 * 
 * Run a sample job in AUbatch:
 * > run ./jobs/job1 20 4
 * 
 * Run a sample benchmark in AUbatch:
 * > test ./benchmarks fcfs 4 6 30 90
 * 
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <string.h>
#include <time.h>
#include "../include/aubatch.h"

enum policies policy;

/* Global Variables */
pthread_mutex_t cmd_queue_lock;   /* Lock for critical sections */
pthread_cond_t cmd_buf_not_full;  /* Condition variable for buf_not_full */
pthread_cond_t cmd_buf_not_empty; /* Condition variable for buf_not_empty */
u_int buf_head;
u_int buf_tail;
u_int count;
struct Job **cmd_buffer[CMD_BUF_SIZE * sizeof(struct Job)];
struct Metrics accumulator;
char *policy_string;
int user_wait_time;

int main()
{
  int res1, res2, i;
  pthread_t cmd_scheduler_thread, dispatcher_thread;

  /* Initilize variables */
  count = 0;
  buf_head = 0;
  buf_tail = 0;
  policy = FCFS;
  policy_string = malloc(POLICY_LEN * sizeof(char));
  strcpy(policy_string, "FCFS");
  accumulator.num_of_jobs = 0;

  for (i = 0; i < CMD_BUF_SIZE; i++)
  {
    cmd_buffer[i] = malloc(sizeof(struct Job));
  }

  /* create Threads */
  res1 = pthread_create(&cmd_scheduler_thread, NULL, cmd_line_parser, NULL);
  res2 = pthread_create(&dispatcher_thread, NULL, dispatcher, NULL);

  if (res1 != 0 || res2 != 0)
  {
    perror("Thread creation failed");
    exit(1);
  }

  /* Initialize the lock the two condition variables */
  pthread_mutex_init(&cmd_queue_lock, NULL);
  pthread_cond_init(&cmd_buf_not_full, NULL);
  pthread_cond_init(&cmd_buf_not_empty, NULL);

  /* Wait here until threads complete */
  res1 = pthread_join(cmd_scheduler_thread, NULL);
  res2 = pthread_join(dispatcher_thread, NULL);

  if (res1 != 0 || res2 != 0)
  {
    perror("Thread join failed");
    return 1;
  }

  return 0;
}
