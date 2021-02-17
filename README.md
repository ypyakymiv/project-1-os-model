Yuri Yakymiv
Section 1

Compile the program by using the make file:

make

The simulation will read in input from a file called sim.conf
and output stats and a log to stats.txt and log.txt.

The following modules are responsible for the following functionality:

input.c - reading in input and setting default values
output.c - outputing statistic information
log.c - outputing log debug information
simulation.c - the core simulation logic
utils.c - core utils that have been developed in previous projects
priority_heap.h - a simple priority heap implementation
