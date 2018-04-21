#!/bin/bash
set -e
echo "Checking CPU info in /proc/cpuinfo"
clockfrequency=`cat /proc/cpuinfo | grep "cpu MHz" | head -1|awk '{print $4}'`
echo "Clock frequency: $clockfrequency"
rdtscpcheck=`cat /proc/cpuinfo | grep "flags" | head -1 | tr ' ' '\n'| grep "rdtscp"`
if [ $rdtscpcheck ]
then
	make
	./main "$clockfrequency"
else
	echo "No rdtscp. Exiting"
fi