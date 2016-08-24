# Process Scheduler Emulator

A program which emulates the process scheduler in operating systems.
So far the program is entirely in C++.

The current stable version of the scheduler is: **Scheduler1.2**.

##Input and Output File Formatting
Scheduler1.2 onwards allow the user to specify input and output file names through the console. Previous to this the input file mush have the name '*inputProcesses.txt*', and the output file would have the name '*outputProcesses.txt*'.

Given an example input .txt file:
```
A 0 5
B 2 3
C 5 2
```
The first column contains the process label, that is to say: this example shows three processes with labels `A`, `B` and `C`. The second column column contains the process arrival time, and the third shows the process length, that is to say: this example shows a process with label `A`, an arrival time of `0` and a length of `5`.

The process label is interpreted as string type should one wish to use more meaningful labels.
The arrival time and length are interpreted as integer type. The units of time are arbitrary.

There are no a priori restrictions on the number of processes the scheduler can handle.

The output file contains an interpreted process input, for this example that would be:
```
** INTERPRETED PROCESS INPUT **
     process: A     B     C        
arrival time: 0     2     5        
      length: 5     3     2         
```
The output file also contains an execution timeline based on each algorithm, for this example the FCFS timeline would be:
```
** FCFS EXECUTION TIMELINE **
     process: A     B     C     END   
        time: 0     5     8     10   
```
The timeline shows the process and the time it started executing, that is to say: this timeline shows that between time `0` and time 4 (time is of integer type) process `A` was executing, then at time `5` process `B` started executing till time 7.

## Scheduler1.2
Scheduler 1.2 implements FSFC, SJF and Round Robin scheduling. Example input and output files ([*inputProcesses.txt*](Scheduler1.2/inputProcesses.txt) and [*outputProcesses.txt*](Scheduler1.2/outputProcesses.txt)) are included in the repository folder. Furthermore, an image ([*roundRobinExample.png*](Scheduler1.2/roundRobinExample.png)) [1], is also included in the repository folder.

[1] - Created by Wikipedia user [Maxtremus](https://en.wikipedia.org/wiki/Round-robin_scheduling#/media/File:Round-robin_schedule_quantum_3.png "Round Robin example image credits")

The image is a diagrmmatic representation of the execution timeline that corresponds to the example input and output files mentioned above.

The Round Robin scheduler time quantum is a preprocessor directive specified on line 9 of the [*main.cpp()*](Scheduler1.2/main.cpp) and can be changed as the user desires. By default the value is at 3.

An interesting feature of this Round Robin implementation is the way it deals with gaps between processes. Given the following input, and assuming a time quantum of 3:

```
A 1 4
B 2 3
C 3 2
D 11 2
```

the Round Robin execution timeline is:

```
** ROUND ROBIN EXECUTION TIMELINE **
     process: NONE  A     B     C     A     NONE  D     END   
        time: 0     1     4     7     9     10    11    13    
```
Notice that at time 9 process A begins executing again rather than waiting for process D to arrive. Since process A has a time of 1 left and the time quantum is 3, A finishes executing. At this point, time 10, D has not arrived and processes A, B and C have finished executing - the CPU is now idle and waits for D to arrive.

This example is given in the files [*inputProcesses2.txt*](Scheduler1.2/inputProcesses2.txt) and [*outputProcesses2.txt*](Scheduler1.2/outputProcesses2.txt) in the Scheduler1.2 folder in this repository.

## Scheduler1.1
Scheduler 1.1 implements FSFC and SJF scheduling. Example input and output files ([*inputProcesses.txt*](Scheduler1.1/inputProcesses.txt) and [*outputProcesses.txt*](Scheduler1.1/outputProcesses.txt)) are included in the Scheduler1.1 repository folder.

There are a number of minor differences between Scheduler1.0 and Scheduler1.1 such as variable and attribute names. These differences were implemented in the development between 1.0 and 1.1 to allow for more scheduling algorithms to be included while maintaining consistency and code readability.
The user specified enquiry time functionality was also removed from 1.1 as this was mainly used for debugging purposes in early development.

## Scheduler1.0
Scheduler 1.0 implements FSFC scheduling and allows the user to enquire by means of console I/O which process was running at any time. Example input and output files ([*inputProcesses.txt*](Scheduler1.0/inputProcesses.txt) and [*outputProcesses.txt*](Scheduler1.0/outputProcesses.txt))) are included in the Scheduler1.0 repository folder.

## To do
- comment code
