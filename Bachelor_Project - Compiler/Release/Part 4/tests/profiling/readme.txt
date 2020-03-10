the .log files were generated using the command: 
valgrind --tool=callgrind --dump-instr=yes --collect-jumps=yes --branch-sim=yes --cache-sim=yes --simulate-hwpref=yes ./knapsack

This tool is used for simulating an execution of the pogram. Because it is a simulation, 
some of the stats generated are simply estimations. 

The log files can be viewed using kcachegrind knapsack.log

O_Knapsack.src use n = 300 to reduce the runtime as the profiler makes it a lot slower.
knapsackOpt is compiled using the options -nc and -ss.
