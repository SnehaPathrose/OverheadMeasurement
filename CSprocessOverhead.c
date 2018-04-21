#define _GNU_SOURCE
#include <sched.h>
#include <stdlib.h>
#include "main.h"

void measure_cspoverhead()
{
  int   i, j, k, limit, pp[2], pc[2], pid; 
  uint64_t start, end, temp; 
  unsigned cycles_low, cycles_high, cycles_low1, cycles_high1;
  
  // pin the process to one core
  cpu_set_t my_set;
  struct sched_param sp;
  CPU_ZERO(&my_set);
  CPU_SET(0, &my_set);
  sched_setaffinity(0, sizeof(my_set), &my_set);
  sp.sched_priority = sched_get_priority_max(SCHED_FIFO);
  sched_setscheduler(0, SCHED_FIFO, &sp);
  
  pipe(pp);
  pipe(pc);

  //flush the data cache
  __asm__  volatile (
          "CPUID\n\t" 
          "RDTSC\n\t" 
          "mov %%edx, %0\n\t"
          "mov %%eax, %1\n\t": 
          "=r" (cycles_high), "=r" (cycles_low):: 
          "%rax", "%rbx", "%rcx", "%rdx"); 
  __asm__   volatile ( 
          "RDTSCP\n\t" 
          "mov %%edx, %0\n\t"
          "mov %%eax, %1\n\t"
          "CPUID\n\t": 
          "=r" (cycles_high1), "=r" (cycles_low1):: 
          "%rax", "%rbx", "%rcx", "%rdx"); 
  
  __asm__   volatile ("CPUID\n\t" 
          "RDTSC\n\t" 
          "mov %%edx, %0\n\t"
          "mov %%eax, %1\n\t": 
          "=r" (cycles_high), "=r" (cycles_low):: 
          "%rax", "%rbx", "%rcx", "%rdx"); 
  __asm__   volatile ( 
          "RDTSCP\n\t" 
          "mov %%edx, %0\n\t"
          "mov %%eax, %1\n\t"
          "CPUID\n\t": 
          "=r" (cycles_high1), "=r" (cycles_low1):: 
          "%rax", "%rbx", "%rcx", "%rdx");

  /*take the actual measurement in 100 samples - 
  */
  for   (j=0; j<BOUND_OF_LOOP; j++) 
  {      
      if(j==0)
        limit=5;
      else
        limit=1;

      for(k=0;k<limit;k++)
      {
        pid = fork(); 
        if(pid==0)
        { 
          read(pp[0],&start,sizeof(start));
          __asm__   volatile ("RDTSCP\n\t" 
                "mov %%edx, %0\n\t"
                "mov %%eax, %1\n\t"
                "CPUID\n\t": 
                "=r" (cycles_high1), "=r" (cycles_low1):: 
                "%rax", "%rbx", "%rcx", "%rdx");
          end   = (((uint64_t)cycles_high1 << 32) | cycles_low1 );
          temp=end-start;
          write(pc[1],&temp,sizeof(temp));
          exit(0);
        }
        else
        {
          __asm__   volatile ("CPUID\n\t" 
                        "RDTSC\n\t" 
                        "mov %%edx, %0\n\t"
                        "mov %%eax, %1\n\t": 
                        "=r" (cycles_high), "=r" (cycles_low):: 
                        "%rax", "%rbx", "%rcx", "%rdx"); 
          start = (((uint64_t)cycles_high << 32)  | cycles_low );
          write(pp[1],&start,sizeof(start));
          read(pc[0],&temp,sizeof(temp));
          waitpid(pid, NULL, 0);
          
        }  
      } 
      cstimes[j]=temp;
    } 
  }       