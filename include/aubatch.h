typedef unsigned int u_int; 

#define POLICY_LEN 10 /* Length for scheduling policy strings */
#define CMD_BUF_SIZE 1000 

struct Job {
  int job_id;
  char filename[50];
  char exec_time[25];
  char pri[5];
  clock_t arrival_time;
  char status[25];
  int cpu_time;
  int finish_time;
  int wait_time;
  int response_time;
};

enum policies {FCFS, SJF, priority}; 

void *dispatcher(void *ptr);
int *scheduler(char **args);
void *cmd_line_parser();
