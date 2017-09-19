# Process Scheduler Emulator

A program which emulates the process scheduler in operating systems.
So far the program is entirely in C++.

The current stable version of the scheduler is: **Scheduler1.3**.

This work was completed in **September 2016**.

## Running the Scheduler
The versionfolders in this repository each contain a source code file `main.cpp` which can be compiled. The scheduler requires an input `.txt` file, the format of which is explained below.

(Note: Scheduler1.2 onwards allow the user to specify input and output file names through the console. Previous to this the input file must have the name `inputProcesses.txt`, and the output file would have the name `outputProcesses.txt`.)

Given an example input `.txt` file:
```
A 0 5
B 2 3
C 5 2
D 12 3
```
The first column contains the process label, the second column column contains the process arrival time and the third contains the process length, that is to say: this example shows a process with label `A`, an arrival time of `0` and a length of `5`.

The process label is interpreted as string type should one wish to use more meaningful labels.
The arrival time and length are interpreted as integer type. The units of time are arbitrary.

The input file can contain the processes in any order and there are no a priori restrictions on the number of processes the scheduler can handle.

The output file contains an interpreted process input, for this example that would be:
```
** INTERPRETED PROCESS INPUT **
         process: A     B     C     D     
    arrival time: 0     2     5     12    
          length: 5     3     2     3        
```
The output file also contains an execution timeline and average waiting and turnaround time based on each algorithm; for this example the FCFS timeline would be:

```
** FCFS EXECUTION TIMELINE **
         process: A     B     C     NONE  D     END   
            time: 0     5     8     10    12    15    
   ~waiting time: 1.5
~turnaround time: 4.75
```
The timeline shows the process that was executing and the time that it started executing, that is to say: this timeline shows that between time `0` and time 5 process `A` was executing, then at time `5` process `B` started executing till time 8. Process `C` starts executing at time `8` and is of length 2, thus at time `10` it has finished executing and the CPU is idle because process `D` does not arrive till time `12`. The idleness of the CPU is represented by a process label of `NONE` while the execution of all processes is represented by a process label of `END` (both of which are specified as preprocessor directives and can be changed as the user desires). The preceding tilde indicates that the waiting and turnaround time are averages values.

## Scheduler1.3
An extension of Scheduler 1.2, Scheduler1.3 implements average waiting and turnaround times for all scheduling algorithms.

## Scheduler1.2
Scheduler 1.2 implements FCFS, SJF and Round Robin scheduling. Example input and output files ([*inputProcesses.txt*](Scheduler1.2/inputProcesses.txt) and [*outputProcesses.txt*](Scheduler1.2/outputProcesses.txt)) are included in the repository folder. Furthermore, an image ([*roundRobinExample.png*](Scheduler1.2/roundRobinExample.png)) [1], is also included in the repository folder.

[1] - Created by Wikipedia user [Maxtremus](https://en.wikipedia.org/wiki/Round-robin_scheduling#/media/File:Round-robin_schedule_quantum_3.png "Round Robin example image credits")

The image is a diagrmmatic representation of the execution timeline that corresponds to the example input and output files mentioned above.

The Round Robin scheduler time quantum is a preprocessor directive specified on line 10 of the [*main.cpp()*](Scheduler1.2/main.cpp) and can be changed as the user desires. By default the value is at 3.

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
         process: A     B     C     A     NONE  D     END   
            time: 0     3     6     8     10    12    15      
```
Notice that at time `9` process `A` begins executing again rather than waiting for process `D` to arrive. Since process `A` has a time of 1 left and the time quantum is 3, `A` finishes executing. At this point, time `10`, `D` has not arrived and processes `A`, `B` and `C` have finished executing - the CPU is now idle and waits for `D` to arrive.

This example is given in the files [*inputProcesses2.txt*](Scheduler1.2/inputProcesses2.txt) and [*outputProcesses2.txt*](Scheduler1.2/outputProcesses2.txt) in the Scheduler1.2 folder in this repository.

## Scheduler1.1
Scheduler 1.1 implements FSFC and SJF scheduling. Example input and output files ([*inputProcesses.txt*](Scheduler1.1/inputProcesses.txt) and [*outputProcesses.txt*](Scheduler1.1/outputProcesses.txt) are included in the Scheduler1.1 repository folder.

There are a number of minor differences between Scheduler1.0 and Scheduler1.1 such as variable and attribute names. These differences were implemented in the development between 1.0 and 1.1 to allow for more scheduling algorithms to be included while maintaining consistency and code readability.
The user specified enquiry time functionality was also removed from 1.1 as this was mainly used for debugging purposes in early development.

## Scheduler1.0
Scheduler 1.0 implements FSFC scheduling and allows the user to enquire by means of console I/O which process was running at any time. Example input and output files ([*inputProcesses.txt*](Scheduler1.0/inputProcesses.txt) and [*outputProcesses.txt*](Scheduler1.0/outputProcesses.txt))) are included in the Scheduler1.0 repository folder.
