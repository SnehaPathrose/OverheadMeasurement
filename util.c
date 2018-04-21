#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "util.h"

void generateCSV(char *filename) 
{
    int range = 100;
    int freqdist[range][2];
    int maxval = 0, minval = 999;
    int freqminindex = 0;
    int freqmaxindex = 0;
    int i;
    FILE *fp;
    
    // Initialise the frequency distribution array
    for (i = 0; i < range; i++) {
        freqdist[i][0] = 0;
        freqdist[i][1] = 0;
    }

    // Find max and min value of the distribution
    for (i = 0; i < range; i++) {
        if(min_values[i] > maxval) {
            maxval = min_values[i];
        }
        if (min_values[i] < minval) {
            minval = min_values[i];
        }
    }


    // Add value in distribution
    for (i = 0; i < range; i++) 
    {
        freqdist[i][0] = i;
        freqdist[i][1] = min_values[i];
    }

    
    fp = fopen(filename, "w");
    if (fp == NULL) 
    {
        printf("Can't open output file %s!\n",filename);
        exit(1);
    }
    
    for (i = 0; i < range; i++)
        fprintf(fp, "%d,%d\n", freqdist[i][0], freqdist[i][1]);
    fclose(fp);
}

uint64_t analyze_times()
{
    int i, j, count, maxcount=0;
    uint64_t max_dev = 0, min_time = 0, max_time = 0, temp, mostrepeatvar;
    uint64_t *temp_values;

    //find minimum and maximum deviation in a sample
    for   (j=0; j<BOUND_OF_LOOP; j++) 
    { 
        max_dev = 0; 
        min_time = 0; 
        max_time = 0; 
        for   (i =0; i<SIZE_OF_STAT; i++) 
        { 
            //printf("%lu ",times[j][i]);
            if ((min_time == 0)||(min_time > times[j][i])) 
                  min_time = times[j][i]; 
            if (max_time < times[j][i]) 
                  max_time = times[j][i]; 
        } 
 
        max_dev = max_time - min_time; 
        min_values[j] = min_time; 
 
        printf("sample no:%d\tmax_deviation: %lu\tmin clock cycle: %lu\n", j, max_dev, min_time); 
    }

    //find most occuring minimum value
    temp_values = malloc(BOUND_OF_LOOP*sizeof(uint64_t)); 
    if (!temp_values) 
    { 
        printf("unable to allocate memory for temp_values\n"); 
        return 0; 
    }
    
    for (j=0; j<BOUND_OF_LOOP; j++)
    {
        temp_values[j] = min_values[j];
    }

    //sort the array 
    for (i=0; i< (BOUND_OF_LOOP - 1); i++)
    {
        for (j=0; j< (BOUND_OF_LOOP - i - 1); j++)
        {
            if (temp_values[j] > temp_values[j+1])
            {
                temp = temp_values[j];
                temp_values[j] = temp_values[j+1];
                temp_values[j+1] = temp;
            }
        }
    } 

    //find the most repeated one
    for (i=0; i< BOUND_OF_LOOP ; i+=j-i)
    {
        count = 0;
        //printf("\ni= %d\n",i);
        for (j=i; j< BOUND_OF_LOOP; j++)
        {
            //printf("j= %d\n",j);
            if (temp_values[i] == temp_values[j])
                count++;
            else
                break;
        }
        if (count > maxcount)
        {
            maxcount = count;
            mostrepeatvar = temp_values[i];
        }


    }

    printf("\nMost repeated min clockcycles: %lu\tcount: %d\n",mostrepeatvar,maxcount);
    return mostrepeatvar;
}

uint64_t analyze_stimes()
{
    int i, j, count, maxcount=0;
    uint64_t max_dev = 0, min_time = 0, max_time = 0, temp, mostrepeatvar;
    uint64_t *temp_values;

    //find minimum and maximum deviation in a sample
    for   (j=0; j<BOUND_OF_LOOP; j++) 
    { 
        
        if ((min_time == 0)||(min_time > cstimes[j])) 
            min_time = cstimes[j]; 
        
        min_values[j] = cstimes[j]; 
 
        printf("sample no:%d\tclock cycles: %lu\n", j, cstimes[j]); 
    }

    printf("\nMinimum clock cycles: %lu\t\n",min_time);
    return min_time;
}