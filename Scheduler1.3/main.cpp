#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>

#define NO_PROCESS_LABEL "NONE" // The literal used as a label to describe the state when no process is executing
#define EXECUTION_COMPLETE_LABEL "END" // The literal used as a label to describe the state when all processes have finished executing
#define TIME_QUANTUM 3 // Time qauntum for Round Robin scheduling

using namespace std;

struct process{
  string label;
  int arrivalTime;
  int length;
  int timeRemaining; // Only used for Round Robin algorithm
  int timeLastExecuted; // Only used for Round Robin waiting and turnaround times
};

struct timelineNode{
  string label;
  int startAtTime;
  int finishAtTime;
};

void sortProcessDirectory(vector<process>& unsortedProcesses);
void getFCFSTimeline(const vector<process>& sortedProcesses, vector<timelineNode>& timeline, float& waitingTime);
void getSJFTimeline(const vector<process>& sortedProcesses, vector<timelineNode>& timeline, float& waitingTime);
bool RRTimeRemainingChecker(const vector<process>& SortedProcesses, int UpToProcess);
void getRRTimeline(vector<process>& sortedProcesses, const int timeQuantum, vector<timelineNode>& timeline, float& waitingTime);


int main(){

  // ____________________INPUT & OUTPUT FILES____________________

  string inputFileName;
  cout << endl << "Enter the name of the input file containing the processes: " << endl;
  cin >> inputFileName;
  cout << endl;
  ifstream processQueue; // Input file containing processes
  processQueue.open(inputFileName.c_str());
  if(!processQueue.is_open()){
    cout << "Could not open process queue. Program will terminate." << endl;
    exit(EXIT_FAILURE);
  }

  string outputFileName;
  cout << "Enter the name for the output file: " << endl;;
  cin >> outputFileName;
  cout << endl;
  ofstream processRun; // Output file to contain process execution timelines
  processRun.open(outputFileName.c_str());
  if(!processRun.is_open()){
    cout << "Could not open process run order. Program will terminate." << endl;
    exit(EXIT_FAILURE);
  }

  vector<process> processDirectory; // Vector which will contain all input processe
  process additiveProcess; // Element to add to vector
  while(processQueue >> additiveProcess.label >> additiveProcess.arrivalTime >> additiveProcess.length){
    additiveProcess.timeRemaining = additiveProcess.length; // Only used for Round Robin algorithm
    additiveProcess.timeLastExecuted = additiveProcess.arrivalTime; // Only used for Round Robin waiting and turnaround times
    processDirectory.push_back(additiveProcess);
  }

  processRun << "** PROCESS INPUT **" << endl;
  for(int i = 0; i < processDirectory.size(); i++){
    processRun << processDirectory[i].label << " " << processDirectory[i].arrivalTime << " " << processDirectory[i].length << endl;
  } // Print onto output file: input file

  processRun << endl << "** INTERPRETED PROCESS INPUT **" << endl;
  processRun << "         process: ";
  for(int i = 0; i < processDirectory.size(); i++){
    processRun << left << setw(6) << processDirectory[i].label;
  }
  processRun << endl;
  processRun << "    arrival time: ";
  for(int i = 0; i < processDirectory.size(); i++){
    processRun << left << setw(6) << processDirectory[i].arrivalTime;
  }
  processRun << endl;
  processRun << "          length: ";
  for(int i = 0; i < processDirectory.size(); i++){
    processRun << left << setw(6) << processDirectory[i].length;
  } // Print onto output file: process information in a clear format


// ____________________TIMELINES____________________

  sortProcessDirectory(processDirectory); // Sort processes based on arrival time

  processRun << endl << endl << "** PROCESS ARRIVAL ORDER **" << endl;
  processRun << "         process: ";
  for(int i = 0; i < processDirectory.size(); i++){
    processRun << left << setw(6) << processDirectory[i].label;
  } // Print onto output file: process arrival order since processes were not sorted previous to this

  // Calculate total execution time and number of processes - used for waiting and turnaround times
  float totalExecutionTime = 0;
  for(int i = 0; i < processDirectory.size(); i++){
    totalExecutionTime += processDirectory[i].length;
  }
  float processNumber = processDirectory.size();


// ____________________FCFS____________________

  vector<timelineNode> FCFSTimeline;
  float FCFSWaitingTime = 0;
  float FCFSAverageWaitingTime;
  float FCFSAverageTurnaroundTime;
  getFCFSTimeline(processDirectory, FCFSTimeline, FCFSWaitingTime); // Create FCFS timeline

  // Print onto output file: timline
  processRun << endl << endl << "** FCFS EXECUTION TIMELINE **" << endl;
  processRun << "         process: ";
  for(int i = 0; i < FCFSTimeline.size(); i++){
    processRun << left << setw(6) << FCFSTimeline[i].label;
  }
  processRun << endl;
  processRun << "            time: ";
  for(int i = 0; i < FCFSTimeline.size(); i++){
    processRun << left << setw(6) << FCFSTimeline[i].startAtTime;
  }

  // Print onto output file: waiting and turnaround times
  FCFSAverageWaitingTime = FCFSWaitingTime/processNumber;
  processRun << endl << "   ~waiting time: " << FCFSAverageWaitingTime;
  FCFSAverageTurnaroundTime = (FCFSWaitingTime + totalExecutionTime)/processNumber;
  processRun << endl << "~turnaround time: " << FCFSAverageTurnaroundTime;


// ____________________SJF____________________

  vector<timelineNode> SJFTimeline;
  float SJFWaitingTime = 0;
  float SJFAverageWaitingTime;
  float SJFAverageTurnaroundTime;
  getSJFTimeline(processDirectory, SJFTimeline, SJFWaitingTime); // Create SJF timeline

  // Print onto output file: timline
  processRun << endl << endl << "** SJF EXECUTION TIMELINE **" << endl;
  processRun << "         process: ";
  for(int i = 0; i < SJFTimeline.size(); i++){
    processRun << left << setw(6) << SJFTimeline[i].label;
  }
  processRun << endl;
  processRun << "            time: ";
  for(int i = 0; i < SJFTimeline.size(); i++){
    processRun << left << setw(6) << SJFTimeline[i].startAtTime;
  }

  // Print onto output file: waiting and turnaround times
  SJFAverageWaitingTime = SJFWaitingTime/processNumber;
  processRun << endl << "   ~waiting time: " << SJFAverageWaitingTime;
  SJFAverageTurnaroundTime = (SJFWaitingTime + totalExecutionTime)/processNumber;
  processRun << endl << "~turnaround time: " << SJFAverageTurnaroundTime;


// ____________________ROUND ROBIN____________________

  vector<timelineNode> RRTimeline;
  float RRWaitingTime = 0;
  float RRAverageWaitingTime;
  float RRAverageTurnaroundTime;
  getRRTimeline(processDirectory, TIME_QUANTUM, RRTimeline, RRWaitingTime); // Create Round Robin timeline

  // Print onto output file: timline
  processRun << endl << endl << "** ROUND ROBIN EXECUTION TIMELINE **" << endl;
  processRun << "         process: ";
  for(int i = 0; i < RRTimeline.size(); i++){
    processRun << left << setw(6) << RRTimeline[i].label;
  }
  processRun << endl;
  processRun << "            time: ";
  for(int i = 0; i < RRTimeline.size(); i++){
    processRun << left << setw(6) << RRTimeline[i].startAtTime;
  }

  // Print onto output file: waiting and turnaround times
  RRAverageWaitingTime = RRWaitingTime/processNumber;
  processRun << endl << "   ~waiting time: " << RRAverageWaitingTime;
  RRAverageTurnaroundTime = (RRWaitingTime + totalExecutionTime)/processNumber;
  processRun << endl << "~turnaround time: " << RRAverageTurnaroundTime;
  processRun << endl << "    time quantum: " << TIME_QUANTUM;


// ____________________INPUT & OUTPUT FILES____________________

  processQueue.close();
  processRun.close();

  cout << "Execution succesful." << endl;
  return 0;
}


void sortProcessDirectory(vector<process>& unsortedProcesses){
  for(int j = 0; j < unsortedProcesses.size() - 1; j++){ // bubble sort
    bool didSwap = false; // set if a swap occurs
    for(int i = 1; i < unsortedProcesses.size(); i++){
      if(unsortedProcesses[i-1].arrivalTime > unsortedProcesses[i].arrivalTime){
        process tmpProcess = unsortedProcesses[i-1];
        unsortedProcesses[i-1] = unsortedProcesses[i];
        unsortedProcesses[i] = tmpProcess;
        didSwap = true;
      }
    }
    if(!didSwap){ // if no swap occured in a single pass, the vector is sorted
      j = unsortedProcesses.size() + 1;
    }
  }
}
void getFCFSTimeline(const vector<process>& sortedProcesses, vector<timelineNode>& timeline, float& waitingTime){
  int cumulativeTime = 0;
  waitingTime = 0.0;
  timelineNode additiveNode;
  for(int i = 0; i < sortedProcesses.size(); i++){
    if(sortedProcesses[i].arrivalTime > cumulativeTime){
      additiveNode.label = NO_PROCESS_LABEL;
      additiveNode.startAtTime = cumulativeTime;
      additiveNode.finishAtTime = sortedProcesses[i+1].arrivalTime; //No risk in accessing non-existing elements since a NO_PROCESS_LABEL cannot occur as the last 'process'
      timeline.push_back(additiveNode);
      cumulativeTime = sortedProcesses[i].arrivalTime;
    }
    additiveNode.label = sortedProcesses[i].label;
    additiveNode.startAtTime = cumulativeTime;
    additiveNode.finishAtTime = cumulativeTime + sortedProcesses[i].length;
    timeline.push_back(additiveNode);
    waitingTime += (cumulativeTime - sortedProcesses[i].arrivalTime);
    cumulativeTime += sortedProcesses[i].length;
  }
  additiveNode.label = EXECUTION_COMPLETE_LABEL;
  additiveNode.startAtTime = cumulativeTime;
  additiveNode.finishAtTime = cumulativeTime;
  timeline.push_back(additiveNode);
}
void getSJFTimeline(const vector<process>& sortedProcesses, vector<timelineNode>& timeline, float& waitingTime){
  int cumulativeTime = 0;
  timelineNode additiveNode;
  vector<process> waitingProcesses;
  int waitingProcessesAdded = 0;
  int overallIterator = 0;
  waitingTime = 0.0;
  // bool processesAreWaiting; //add in for case when only one process is waiting. No need to sort vector etc.
  for(overallIterator; overallIterator < sortedProcesses.size(); overallIterator++){
    if( (sortedProcesses[overallIterator].arrivalTime > cumulativeTime) && (waitingProcesses.size() == 0)){
      additiveNode.label = NO_PROCESS_LABEL;
      additiveNode.startAtTime = cumulativeTime;
      additiveNode.finishAtTime = sortedProcesses[overallIterator].arrivalTime; //No risk in accessing non-existing elements since a NO_PROCESS_LABEL cannot occur as the last 'process'
      timeline.push_back(additiveNode);
      cumulativeTime = sortedProcesses[overallIterator].arrivalTime;
    }
    waitingProcessesAdded = 0;
    for(int j = overallIterator; j < sortedProcesses.size(); j++){ //need to find alternative stable way, for loop not needed since processes are already sorted
      if (sortedProcesses[j].arrivalTime <= cumulativeTime){
        waitingProcesses.push_back(sortedProcesses[j]);
        waitingProcessesAdded++;
      }
    }
    for(int j = 0; j < waitingProcesses.size() - 1; j++){
      bool didSwap = false;
      for(int k = 1; k < waitingProcesses.size(); k++){
        if(waitingProcesses[k-1].length > waitingProcesses[k].length){
          process tmpProcess = waitingProcesses[k-1];
          waitingProcesses[k-1] = waitingProcesses[k];
          waitingProcesses[k] = tmpProcess;
          didSwap = true;
        }
      }
      if(!didSwap){
        j = waitingProcesses.size() + 1;
      }
    }
    additiveNode.label = waitingProcesses[0].label;
    additiveNode.startAtTime = cumulativeTime;
    additiveNode.finishAtTime = cumulativeTime + waitingProcesses[0].length;
    timeline.push_back(additiveNode);
    waitingTime += (cumulativeTime - waitingProcesses[0].arrivalTime);
    cumulativeTime += waitingProcesses[0].length;
    waitingProcesses.erase(waitingProcesses.begin());

    overallIterator += (waitingProcessesAdded - 1);
  }

  if(waitingProcesses.size() != 0){
    for(int p = 0; p < waitingProcesses.size(); p++){
      additiveNode.label = waitingProcesses[p].label;
      additiveNode.startAtTime = cumulativeTime;
      additiveNode.finishAtTime = cumulativeTime + waitingProcesses[p].length;
      timeline.push_back(additiveNode);
      waitingTime += (cumulativeTime - waitingProcesses[p].arrivalTime);
      cumulativeTime += waitingProcesses[p].length;
    }
  }
  additiveNode.label = EXECUTION_COMPLETE_LABEL;
  additiveNode.startAtTime = cumulativeTime;
  additiveNode.finishAtTime = cumulativeTime;
  timeline.push_back(additiveNode);
}
bool RRTimeRemainingChecker(const vector<process>& SortedProcesses, int UpToProcess){
  bool TimeRemaining = false;
  for(int i = 0; i < UpToProcess; i++){
    if(SortedProcesses[i].timeRemaining > 0){
      TimeRemaining = true;
      i = UpToProcess + 1;
    }
  }
  return TimeRemaining;
}
void getRRTimeline(vector<process>& sortedProcesses, const int timeQuantum, vector<timelineNode>& timeline, float& waitingTime){
  int cumulativeTime = 0;
  timelineNode additiveNode;
  waitingTime = 0.0;
  while(RRTimeRemainingChecker(sortedProcesses, (sortedProcesses.size()))){
    for(int i = 0; i < sortedProcesses.size(); i++){
      if( (sortedProcesses[i].arrivalTime > cumulativeTime) && !RRTimeRemainingChecker(sortedProcesses, i)){
        additiveNode.label = NO_PROCESS_LABEL;
        additiveNode.startAtTime = cumulativeTime;
        additiveNode.finishAtTime = sortedProcesses[i].arrivalTime; //No risk in accessing non-existing elements since a NO_PROCESS_LABEL cannot occur as the last 'process'
        timeline.push_back(additiveNode);
        cumulativeTime = sortedProcesses[i].arrivalTime;
      }
      if((sortedProcesses[i].arrivalTime > cumulativeTime) && RRTimeRemainingChecker(sortedProcesses, i)){
        i = - 1;
      }
      else if(sortedProcesses[i].timeRemaining > 0){
        additiveNode.label = sortedProcesses[i].label;
        additiveNode.startAtTime = cumulativeTime;
        waitingTime += (cumulativeTime - sortedProcesses[i].timeLastExecuted);
        if(sortedProcesses[i].timeRemaining >= timeQuantum){
          additiveNode.finishAtTime = cumulativeTime + timeQuantum; //No risk in accessing non-existing elements since a NO_PROCESS_LABEL cannot occur as the last 'process'
          sortedProcesses[i].timeLastExecuted = cumulativeTime + timeQuantum;
          cumulativeTime += timeQuantum;
          sortedProcesses[i].timeRemaining -= timeQuantum;
        }
        else{
          additiveNode.finishAtTime = cumulativeTime + sortedProcesses[i].timeRemaining;
          sortedProcesses[i].timeLastExecuted = cumulativeTime + sortedProcesses[i].timeRemaining;
          cumulativeTime += sortedProcesses[i].timeRemaining;
          sortedProcesses[i].timeRemaining = 0;
        }
        timeline.push_back(additiveNode);
      }
    }
  }
    additiveNode.label = EXECUTION_COMPLETE_LABEL;
    additiveNode.startAtTime = cumulativeTime;
    additiveNode.finishAtTime = cumulativeTime;
    timeline.push_back(additiveNode);
}
