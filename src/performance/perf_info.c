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
 * gcc perf_info.c -o perf_info
 * ./perf_inf
 *
*/

#include <stdio.h>
#include <time.h>
#include "../../include/aubatch.h"

extern struct Metrics accumulator;

void *perf_info(time_t arrival, time_t start, time_t finish)
{
  int jobs;
  float cpu_avg, cpu_time, turnaround_avg, turnaround, wait_avg, response_avg, throughput;

  // add to average:  [(avg * jobs-1) + new_val] / jobs

  /* Increment job count and assign to local jobs variable */
  accumulator.num_of_jobs++;
  jobs = accumulator.num_of_jobs;

  cpu_time = finish - start;
  turnaround = finish - arrival;

  if (jobs == 1)
  {
    //printf("cpu_time: %ld\n", finish - start);
    accumulator.avg_cpu = cpu_time;
    accumulator.avg_turnaround = turnaround;
    accumulator.avg_wait = turnaround - cpu_time;
    accumulator.avg_response = turnaround - cpu_time;
    accumulator.throughput = 1 / turnaround;
  }
  else
  {
    /* Calc Average CPU_Time */
    cpu_avg = accumulator.avg_cpu;
    accumulator.avg_cpu = ((cpu_avg * (jobs - 1)) + cpu_time) / jobs;

    /* Calc Avg Turnaround Time */
    turnaround_avg = accumulator.avg_turnaround;
    accumulator.avg_turnaround = ((turnaround_avg * (jobs - 1)) + turnaround) / jobs;

    /* Calc Avg Waiting Time */
    wait_avg = accumulator.avg_wait;
    accumulator.avg_wait = ((wait_avg * (jobs - 1)) + (turnaround - cpu_time)) / jobs;

    /* Calc Avg Response Time --nonpremptive so will be same as wait time */
    response_avg = accumulator.avg_response;
    accumulator.avg_response = ((response_avg * (jobs - 1)) + (turnaround - cpu_time));

    /* Calculate Throughput Rate */
    turnaround_avg = accumulator.avg_turnaround; // get new calculate turnaround_avg
    accumulator.throughput = 1 / turnaround_avg;
  }

  return 0;
};