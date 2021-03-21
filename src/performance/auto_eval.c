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
 * gcc auto_eval.c -o auto_eval
 * ./auto_eval
 *
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <time.h>
#include "../../include/aubatch.h"

extern u_int policy;
extern char *policy_string;
extern struct Metrics accumulator;

int random_num(int min, int max)
{
  int x;
  x = (rand() % (max - min + 1)) + min;
  return x;
};

void *auto_eval(char **args)
{
  char *dir = args[1];
  char *pol = args[2];
  char *num = args[3];
  char *pri = args[4];
  char *min_cpu = args[5];
  char *max_cpu = args[6];
  char *sched_cmd[4];
  char run_buf[50];

  struct dirent *executable;
  DIR *dir_ptr;
  char pri_str[10], cpu_str[10];
  int pri_int, cpu_int;

  /* set the scheduling algorithm */
  if (strcmp(pol, "fcfs") == 0)
  {
    policy = FCFS;
    strcpy((char *)policy_string, "FCFS");
  }
  else if (strcmp(pol, "sjf") == 0)
  {
    policy = SJF;
    strcpy(policy_string, "SJF");
  }
  else if (strcmp(pol, "priority") == 0)
  {
    policy = priority;
    strcpy(policy_string, "priority");
  }
  else
  {
    printf("Invalid policy.  Policy must be of fcfs, sjf, priority.\n");
    return 0;
  }

  dir_ptr = opendir(dir);
  if (dir_ptr != NULL)
  {
    while ((executable = readdir(dir_ptr)))
    {
      char *name = executable->d_name;
      if (strcmp(name, "..") == 0 || strcmp(name, ".") == 0)
      {
        continue;
      }

      srand(time(0));

      /* Generate a Priority */
      pri_int = random_num(1, atoi(pri));
      sprintf(pri_str, "%d", pri_int);

      /* Generate Execution Time */
      cpu_int = random_num(atoi(min_cpu), atoi(max_cpu));
      sprintf(cpu_str, "%d", cpu_int);

      /* Build command to pass to scheduler */
      snprintf(run_buf, sizeof(run_buf), "./benchmarks/%s%c", name, '\0');

      //*sched_cmd_ptr = &sched_cmd;
      sched_cmd[0] = "run";
      sched_cmd[1] = run_buf;
      sched_cmd[2] = cpu_str;
      sched_cmd[3] = pri_str;

      /* Send the job to the scheduler */
      scheduler(sched_cmd);

      sleep(1);
    }
  }

  closedir(dir_ptr);

  return 0;
}