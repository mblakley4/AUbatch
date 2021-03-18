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
  char status[25];
  float cpu_time;
};

struct Metrics
{
  int num_of_jobs;
  float avg_cpu;
  float avg_turnaround;
  float avg_wait;
  float avg_response;
  float throughput;
};

enum policies
{
  FCFS,
  SJF,
  priority
};

void *dispatcher(void *ptr);
void *scheduler(char **args);
void *cmd_line_parser();
void *perf_info(time_t arrival, time_t start, time_t finish);
void *auto_eval(char **args);
