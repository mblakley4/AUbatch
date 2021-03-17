typedef unsigned int u_int;

#define POLICY_LEN 10 /* Length for scheduling policy strings */
#define CMD_BUF_SIZE 1000

struct Job
{
  int job_id;
  char filename[50];
  char exec_time[25];
  char pri[5];
  char display_time[30];
  time_t arrival_time;
  float start_time;
  float finish_time;
  float response_time;
  float turn_around_time;
  float wait_time;
  float cpu_time;
  char status[25];
};

enum policies
{
  FCFS,
  SJF,
  priority
};

void *dispatcher(void *ptr);
int *scheduler(char **args);
void *cmd_line_parser();
void *perf_info(void *ptr);
