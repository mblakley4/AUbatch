/*
 * COMP 7500/06: Project 3 AUbatch
 * Michael Blakley
 * mzb0155@auburn.edu
 * Auburn University
 * 
 * Compilation Instructions:
 * Compile at project root with $make all, then run $./aubatch
 * Individually compile with 
 * gcc scheduler.c -o scheduler
 * ./scheduler
 *
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../../include/aubatch.h"

extern struct Job *cmd_buffer;
extern u_int policy;
extern u_int buf_head;
extern u_int buf_tail;
extern u_int count;
extern int user_wait_time;

int id_counter = 1;

void calc_user_wait_time(int index)
{
  int val;

  /* initialize user_wait_time */
  user_wait_time = 0;

  while (index > buf_tail)
  {
    if (strcmp(cmd_buffer[index - 1].status, "Waiting") == 0)
    {
      val = atoi(cmd_buffer[index - 1].exec_time);
      user_wait_time += val;
    }
    index--;
  }
}

int fcfs_scheduling()
{
  int val1, val2, i, j, submitted_job_id;
  if (count == 1)
  {
    user_wait_time = 0;
    return 0;
  }

  /* store the submitted job id */
  submitted_job_id = cmd_buffer[buf_head].job_id;

  // insertion sort quick solution - for a queue capped at 1000 should still sort ~max 1ms
  // should still improve this to O(logN) sorting
  for (i = buf_tail; i < buf_head; i++)
  {
    for (j = i; j > buf_tail; j--)
    {
      if (strcmp(cmd_buffer[j - 1].status, "Running") == 0)
      {
        break;
      }
      /* convert values */
      val1 = cmd_buffer[j - 1].arrival_time;
      val2 = cmd_buffer[j].arrival_time;
      // compare
      if (val1 > val2)
      {
        // swap
        struct Job buffer = cmd_buffer[j - 1];
        cmd_buffer[j - 1] = cmd_buffer[j];
        cmd_buffer[j] = buffer;
      }
      else
      {
        break;
      }
    }
  }

  /* find new position of submitted job to determine user_wait_time */
  for (i = buf_tail; i < buf_head; i++)
  {
    if (cmd_buffer[i].job_id == submitted_job_id)
    {
      calc_user_wait_time(i);
    }
  }

  return 0;
};

int sjf_scheduling()
{
  int val1, val2, i, j, submitted_job_id;

  if (count == 1)
  {
    user_wait_time = 0;
    return 0;
  }

  /* store the submitted job id */
  submitted_job_id = cmd_buffer[buf_head].job_id;

  // insertion sort quick solution - for a queue capped at 1000 should still sort ~max 1ms
  // should still improve this to O(logN) sorting
  for (i = buf_tail; i < buf_head; i++)
  {
    for (j = i; j > buf_tail; j--)
    {
      if (strcmp(cmd_buffer[j - 1].status, "Running") == 0)
      {
        break;
      }
      /* convert values */
      val1 = atoi(cmd_buffer[j - 1].exec_time);
      val2 = atoi(cmd_buffer[j].exec_time);
      // compare
      if (val1 > val2)
      {
        // swap
        struct Job buffer = cmd_buffer[j - 1];
        cmd_buffer[j - 1] = cmd_buffer[j];
        cmd_buffer[j] = buffer;
      }
      else
      {
        break;
      }
    }
  }

  /* find new position of submitted job to determine user_wait_time */
  for (i = buf_tail; i < buf_head; i++)
  {
    if (cmd_buffer[i].job_id == submitted_job_id)
    {
      calc_user_wait_time(i);
    }
  }

  return 0;
};

int priority_scheduling()
{
  int val1, val2, i, j, submitted_job_id;

  if (count == 1)
  {
    user_wait_time = 0;
    return 0;
  }

  /* store the submitted job id */
  submitted_job_id = cmd_buffer[buf_head].job_id;
  // insertion sort quick solution - for a queue capped at 1000 should still sort ~max 1ms
  // should still improve this to O(logN) sorting
  for (i = buf_tail; i < buf_head; i++)
  {
    for (j = i; j > buf_tail; j--)
    {
      if (strcmp(cmd_buffer[j - 1].status, "Running") == 0)
      {
        break;
      }

      /* convert values */
      val1 = atoi(cmd_buffer[j - 1].pri);
      val2 = atoi(cmd_buffer[j].pri);
      // compare - lower priority takes precedence
      if (val1 > val2)
      {
        // swap
        struct Job buffer = cmd_buffer[j - 1];
        cmd_buffer[j - 1] = cmd_buffer[j];
        cmd_buffer[j] = buffer;
      }
      else
      {
        break;
      }
    }
  }

  /* find new position of submitted job to determine user_wait_time */
  for (i = buf_tail; i < buf_head; i++)
  {
    if (cmd_buffer[i].job_id == submitted_job_id)
    {
      calc_user_wait_time(i);
    }
  }

  return 0;
};

int *scheduler(char **args)
{
  char *cmd = args[0];
  char *clean_pri = args[3];
  char time_string[9];
  time_t current_time;
  struct tm *time_now;

  /* if cmd is run, create new job */
  if (strcmp(cmd, "run") == 0)
  {
    /* create new_job and it's pointer */
    struct Job new_job;
    //struct Job *new_job_ptr;

    /* assign new_job to it's pointer */
    //new_job_ptr = &new_job;

    /* create pointer to cmd_buffer */
    struct Job **cmd_buffer_ptr = &cmd_buffer;

    /* Construct new job */
    clean_pri[strcspn(clean_pri, "\n")] = 0;
    time(&current_time);
    //printf("time: %s\n", ctime(&current_time));
    time_now = localtime(&current_time);
    strftime(time_string, sizeof(time_string), "%H:%M:%S", time_now);

    new_job.job_id = id_counter;
    strcpy(new_job.filename, args[1]);
    strcpy(new_job.exec_time, args[2]);
    strcpy(new_job.pri, clean_pri);
    strcpy(new_job.display_time, time_string);
    new_job.arrival_time = clock();
    strcpy(new_job.status, "Waiting");

    (*cmd_buffer_ptr)[buf_head] = new_job;

    printf("Job %s was submitted.\n", cmd_buffer[buf_head].filename);

    /* increment the count, id_counter, and buffer head */
    id_counter++;
    count++;
    buf_head++;

    // printf("buffer check: %s\n", cmd_buffer[buf_head].filename);
    // printf("buffer check: %s\n", cmd_buffer[buf_head].exec_time);
    //printf("buffer check: %s\n", cmd_buffer[buf_head].pri);
  }

  // check policy and update cmd_buffer
  if (policy == 0)
  {
    fcfs_scheduling();
  }
  else if (policy == 1)
  {
    sjf_scheduling();
  }
  else if (policy == 2)
  {
    priority_scheduling();
  }
  else
  {
    printf("Invalid policy");
    exit(1);
  }

  return 0;
};