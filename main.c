#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "util.h"

extern void measure_moverhead();
extern void measure_sysoverhead();
extern void measure_csmoverhead();
extern void measure_cspoverhead();
extern void measure_cstoverhead();


void initialize_variables()
{
	int i, j, k;
	times = malloc(BOUND_OF_LOOP*sizeof(uint64_t*)); 
    if (!times) 
  	{ 
          printf("unable to allocate memory for times\n"); 
          exit(1); 
    } 

    for   (j=0; j<BOUND_OF_LOOP; j++) 
  	{ 
        times[j] = malloc(SIZE_OF_STAT*sizeof(uint64_t));
    	if (!times[j]) 
    	{ 
        	printf("unable to allocat memory for times[%d]\n", j); 
        	for (k=0; k<j; k++) 
            	free(times[k]); 
        	exit(1); 
    	} 
	}

	min_values = malloc(BOUND_OF_LOOP*sizeof(uint64_t)); 
    if (!min_values) 
  	{ 
    	printf("unable to allocate memory for min_values\n"); 
        exit(1);
    }

    cstimes = malloc(BOUND_OF_LOOP*sizeof(uint64_t)); 
    if (!cstimes) 
  	{ 
    	printf("unable to allocate memory for cstimes\n"); 
        exit(1);
    } 
}

void free_variables()
{
	int j;
	for   (j=0; j<BOUND_OF_LOOP; j++) 
  		free(times[j]); 
    free(times);
    free(cstimes); 
    free(min_values); 
}

int main(int argc, char **argv)
{
	float clockfrequency;;
	uint64_t mtime, stime, csmtime, csptime, csttime;
	clockfrequency = (atof(argv[1]))/1000;
	initialize_variables();

	//measurement overhead
	printf("Generating 100 samples for measurement overhead...\n");
	measure_moverhead();
	mtime = analyze_times();
	generateCSV("1measurementOverhead.csv");

	//syscall overhead
	printf("\n\nGenerating 100 samples for syscall overhead...\n");
	measure_sysoverhead();
	stime = analyze_times();
	generateCSV("2syscallOverhead.csv");
	
	//ContextSwitch measurement overhead
	printf("\n\nGenerating 100 samples for ContextSwitch measurement overhead...\n");
	measure_csmoverhead();
	csmtime = analyze_times();
	generateCSV("3CSmeasureOverhead.csv");
	

	//ContextSwitch Process overhead
	printf("\n\nGenerating 100 samples for ContextSwitch process overhead...\n");
	measure_cspoverhead();
	csptime = analyze_stimes();
	generateCSV("4CSprocessOverhead.csv");
	

	//ContextSwitch Thread overhead
	printf("\n\nGenerating 100 samples for ContextSwitch thread overhead...\n");
	measure_cstoverhead();
	csttime = analyze_stimes();
	generateCSV("5CSthreadOverhead.csv");
	
	printf("\n\n===============================================Summary=========================================================\n\n");
	printf("Clock Frequency:\t\t\t%f GHz\n",clockfrequency);
	printf("Measurement overhead:\t\t\tClockcycles: %lu\t\t\t\tTime: %f ns\n", mtime,mtime/clockfrequency);
	printf("Syscall overhead:\t\t\tClockcycles: %lu-%lu = %lu\t\t\tTime: %f ns\n", stime, mtime, stime - mtime, (stime - mtime)/clockfrequency);
	printf("ContextSwitch measurement overhead:\tClockcycles: %lu-%lu = %lu\t\tTime: %f ns\n", csmtime, mtime, csmtime - mtime, (csmtime - mtime)/clockfrequency);
	printf("ContextSwitch process overhead:\t\tClockcycles: %lu-%lu-%lu = %lu\tTime: %f ns\n", csptime, csmtime - mtime, mtime, csptime - (csmtime - mtime) - mtime, (csptime - (csmtime - mtime) - mtime)/clockfrequency);
	printf("ContextSwitch thread overhead:\t\tClockcycles: %lu-%lu-%lu = %lu\t\tTime: %f ns\n", csttime, csmtime - mtime, mtime, csttime - (csmtime - mtime) - mtime, (csttime - (csmtime - mtime) - mtime)/clockfrequency);
	printf("\nDetails of samples in CSVs:\n1measurementOverhead.csv\n2syscallOverhead.csv\n3CSmeasureOverhead.csv\n4CSprocessOverhead.csv\n5CSthreadOverhead.csv\n");
	free_variables();
    return 0; 

}
