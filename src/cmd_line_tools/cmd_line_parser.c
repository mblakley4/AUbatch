/*
 * COMP 7500/06: Project 3 AUbatch
 * Michael Blakley
 * mzb0155@auburn.edu
 * Auburn University
 *
 * A large portion of this file was provided as sample code from
 * Dr. Xiao Qin - Dept. of CS & SE - Auburn University
 * 
 * Compilation Instructions:
 * Compile at project root with $make all, then run $./aubatch
 * Individually compile with 
 * gcc cmd_line_parser.c -o cmd_line_parser
 * ./cmd_line_parser
 *
*/

#include <sys/types.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "../../include/aubatch.h"

/* Error Code */
#define EINVAL       1
#define E2BIG        2

#define MAXMENUARGS  4 
// #define CMD_BUF_SIZE 1000 

//enum policies {FCFS, SJF, priority};

extern pthread_mutex_t cmd_queue_lock; 
extern pthread_cond_t cmd_buf_not_full; 
extern pthread_cond_t cmd_buf_not_empty;
extern u_int buf_head;
extern u_int buf_tail;
extern u_int count;
extern struct Job *cmd_buffer;
extern u_int policy;
extern char *policy_string;
extern int user_wait_time;

void menu_execute(char *line, int isargs); 
int cmd_run(int nargs, char **args); 
int cmd_quit(int nargs, char **args); 
void showmenu(const char *name, const char *x[]);
int cmd_helpmenu(int n, char **a);
int cmd_dispatch(char *cmd);
int change_policy(int nargs, char **args);
int cmd_list();


/*
 * The run command - submit a job.
 */
int cmd_run(int nargs, char **args) {

	if (nargs != 4) {
		printf("Usage: run <job> <time> <priority>\n");
		return EINVAL;
	}

  scheduler(args);

  printf("Expected waiting time: %i seconds\n", user_wait_time);
  printf("Scheduling Policy: %s\n", policy_string);
        
  return 0;
}

/*
 * Change scheduling priority
 */
int change_policy(int nargs, char **args) {
  int policy_change = 0;
  char *newPolicy = args[0];

    if (strcmp(newPolicy, "fcfs\n") == 0) {
      if(policy != 0) {
        policy_change = 1;
        policy = FCFS;
        strcpy((char *) policy_string, "FCFS");
      } else {
        printf("The policy is already set to FCFS.\n");
      }
  } else if (strcmp(newPolicy, "sjf\n") == 0) {
    if (policy != 1) {
      policy_change = 1;
      policy = SJF;
      strcpy(policy_string, "SJF");
    } else {
        printf("The policy is already set to SJF.\n");
      }
  } else if (strcmp(newPolicy, "priority\n") == 0) {
    if (policy != 2) {
      policy_change = 1;
      policy = priority;
      strcpy(policy_string, "priority");
    } else {
        printf("The policy is already set to priority.\n");
      }
  }

  if (policy_change == 1) {
    scheduler(args);
    printf("Scheduling policy is switched to: %s. All of the %i waiting jobs have been rescheduled.\n", policy_string, count);
  }

  return 0;
  
}

int cmd_list() {
  int i;
  printf("Total number of jobs in the queue: %i\n", count);
  printf("Scheduling Policy: %s\n", policy_string);
  printf("Name \tCPU_Time \tPri \tArrival_time \tProgress\n");
  for (i = buf_tail; i < buf_head; i++) {
    printf("job%i \t%i \t\t%s \t%.2f \t\t%s\n", cmd_buffer[i].job_id, cmd_buffer[i].cpu_time, cmd_buffer[i].pri, cmd_buffer[i].arrival_time, cmd_buffer[i].status);
  }
  return 0;
}


/*
 * The quit command.
 */
int cmd_quit(int nargs, char **args) {

  // make call to a performance file
	printf("Please display performance information before exiting AUbatch!\n");
        exit(0);
}

/*
 * Display menu information
 */
void showmenu(const char *name, const char *x[])
{
	int ct, i;

	printf("\n");
	printf("%s\n", name);
	
	for (i=ct=0; x[i]; i++) {
		ct++;
	}

	for (i=0; i<ct; i++) {
		printf("    %-36s", x[i]);
		printf("\n");
	}

	printf("\n");
}

static const char *helpmenu[] = {
	"run <job> <time> <priority>:  \n\t\t\t\tsubmit a job named <job>, \n\t\t\t\texecution time is <time>, \n\t\t\t\tpriority is <pri>",
	"list: display the job status",
  "fcfs: change the scheduling policy to FCFS",
  "sjf: change the scheduling policy to SJF",
  "priority: change the scheduling policy to priority",
  "test <benchmark> <policy> <num_of_jobs> <priority_levels> \n\t<min_CPU_time> <max_CPU_time>",
  "quit: exit AUbatch",
	"help: Print help menu",
  NULL
};

int cmd_helpmenu(int n, char **a)
{
	(void)n;
	(void)a;

	showmenu("AUbatch help menu", helpmenu);
	return 0;
}

/*
 *  Command table.
 */
static struct {
	const char *name;
	int (*func)(int nargs, char **args);
} cmdtable[] = {
	/* commands: single command must end with \n */
	{ "help\n",	cmd_helpmenu },
	{ "?\n",	cmd_helpmenu },
	{ "h\n",	cmd_helpmenu },
	{ "run",	cmd_run },
	{ "r",		cmd_run },
	{ "quit\n",	cmd_quit },
	{ "q\n",	cmd_quit },
  { "fcfs\n", change_policy },
  { "sjf\n", change_policy },
  { "priority\n", change_policy },
  { "list\n", cmd_list},
        {NULL, NULL}
};

/*
 * Process a single command.
 */
int cmd_dispatch(char *cmd)
{
	time_t beforesecs, aftersecs, secs;
	u_int32_t beforensecs, afternsecs, nsecs;
	char *args[MAXMENUARGS];
	int nargs=0;
	char *word;
	char *context;
 	int i, result;

	for (word = strtok_r(cmd, " ", &context);
	     word != NULL;
	     word = strtok_r(NULL, " ", &context)) {

		if (nargs >= MAXMENUARGS) {
			printf("Command line has too many words\n");
			return E2BIG;
		}
		args[nargs++] = word;
	}

	if (nargs==0) {
		return 0;
	}

	for (i=0; cmdtable[i].name; i++) {
		if (*cmdtable[i].name && !strcmp(args[0], cmdtable[i].name)) {
			assert(cmdtable[i].func!=NULL);
            
			result = cmdtable[i].func(nargs, args);
			return result;
		}
	}

	printf("%s: Command not found\n", args[0]);
	return EINVAL;
}


/*
 * Command line main loop.
 */
void *cmd_line_parser() {
  printf("Welcome to Michael Blakley's batch job scheduler Version 1.0.\nType 'help' to find more about AUbatch commands.\n");

  char *buffer;
  size_t bufsize = 64;
        
  buffer = (char*) malloc(bufsize * sizeof(char));
  if (buffer == NULL) {
 		perror("Unable to malloc buffer");
 		exit(1);
	}
 
  while (1) {
    pthread_mutex_lock(&cmd_queue_lock);

    while (count == CMD_BUF_SIZE) {
      pthread_cond_wait(&cmd_buf_not_full, &cmd_queue_lock);
    }

    pthread_mutex_unlock(&cmd_queue_lock);

		printf("> ");
		getline(&buffer, &bufsize, stdin);
    
    pthread_mutex_lock(&cmd_queue_lock);

		cmd_dispatch(buffer);

    pthread_cond_signal(&cmd_buf_not_empty);
    pthread_mutex_unlock(&cmd_queue_lock);

	}
  return 0;
}