# Process Scheduler Emulator

A program which emulates the process scheduler in operating systems.
So far the program is entirely in C++.

The current stable version of the scheduler is: **Scheduler1.2**.

##Input File Format
Given an example input .txt file:
```
A 0 5
B 2 3
C 5 2
```
The first column contains the process label, that is to say: this example shows three processes with labels `A`, `B` and `C`. The second column column contains the process arrival time, and the third shows the process length, that is to say: this example shows a process with label `A`, an arrival time of 0 and a length of 5.

The process label is interpreted as string type should one wish to use more meaningful labels.
The arrival time and length are interpreted as integer type.

There are no a priori restrictions on the number of processes the scheduler can handle.

## Scheduler1.2
Scheduler 1.2 implements FSFC, SJF and Round Robin scheduling. Example input and output files (*inputProcesses.txt* and *outputProcesses.txt*) are included in the repository folder. Furthermore, an image (*roundRobinExample.png*) [1], is also included in the repository folder.

[1] - Created by Wikipedia user [Maxtremus](https://en.wikipedia.org/wiki/Round-robin_scheduling#/media/File:Round-robin_schedule_quantum_3.png "Round Robin example image credits")

The image is a diagrmmatic representation of the execution timeline that corresponds to the example input and output files mentioned above.

An interesting feature of this Round Robin implementation is the way it deals with gaps between processes. Given the following input:

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

## Scheduler1.1
Scheduler 1.1 implements FSFC and SJF scheduling. Example input and output files (*inputProcesses.txt* and *outputProcesses.txt*) are included in the repository folder.

There are a number of minor differences between Scheduler1.0 and Scheduler1.1 such as variable and attribute names. These differences were implemented in the development between 1.0 and 1.1 to allow for more scheduling algorithms to be included while maintaining consistency and code readability.
The user specified enquiry time functionality was also removed from 1.1 as this was mainly used for debugging purposes in early development.

## Scheduler1.0
Scheduler 1.0 implements FSFC scheduling and allows the user to enquire by means of console I/O which process was running at any time. Example input and output files (*inputProcesses.txt* and *outputProcesses.txt*) are included in the repository folder.

##Program Features
Features currently part of the program:
- FSFC or FIFO scheduling
- Enquiry of which process was running at a user-specified time
- SJF scheduling

Features to be added to the program:
- Average waiting and turnaround time
- SRemainingJF scheduling
- Export process execution timeline as a graph
