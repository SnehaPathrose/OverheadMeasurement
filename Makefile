overhead: main.c
	gcc -o main main.c measurementOverhead.c util.c syscallOverhead.c CSmeasurementOverhead.c CSprocessOverhead.c CSthreadOverhead.c -pthread -I.

clean: 
	rm -f main
	rm -f *.csv