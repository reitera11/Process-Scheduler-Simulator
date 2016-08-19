# Process Scheduler Emulator

A program which emulates the process scheduler in operating systems.
So far the program is entirely in C++.

## Schedular1.0
Scheduler 1.0 implements FSFC scheduling and allows the user to enquire by means of the console which process was running at any time. Example input and output files (*inputProcesses.txt* and *outputProcesses.txt*) are included in the repository.
The example input file *inputProcesses.txt* contains a small number of processes in the required format, for example:
```
A 0 5
B 2 3
C 5 2
```
The first column contains the process label, that is to say: this example shows three processes with labels `A`, `B` and `C`. The second column column contains the process arrival time, and the third shows the process length, that is to say: this example shows a process with label `A`, an arrival time of 0 and a length of 5.

The process label are interpreted as string types should one wish to include more meaningful labels.
The arrival time and length are interpreted as integer types.

There are no a priori restrictions on the number of processes the scheduler can handle.


Features currently part of the program:
- FSFC or FIFO scheduling
- Enquiry of which process was running at a certain time

Features to be added to the program:
- SJF scheduling
- SRemainingJF sceduling
- Export process execution timeline as a graph

The forthcoming plan is to spend one final session on this project:
- Upgrade version to 1.1
- Implement SJF
- Implement SRJF
