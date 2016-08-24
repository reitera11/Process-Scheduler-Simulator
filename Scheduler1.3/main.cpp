#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>

#define NO_PROCESS_LABEL "NONE" // The literal used as a label to describe the state when no process is executing
#define EXECUTION_COMPLETE_LABEL "END"
#define TIME_QUANTUM 3 // Time qauntum for Round Robin scheduling;

using namespace std;

struct process{
  string label;
  int arrivalTime;
  int length;
  int timeRemaining; // only used for Round Robin algorithm
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

  string inputFileName;
  cout << endl << "Enter the name of the input file containing the processes: " << endl;
  cin >> inputFileName;
  cout << endl;
  ifstream processQueue;
  processQueue.open(inputFileName.c_str());
  if(!processQueue.is_open()){
    cout << "Could not open process queue. Program will terminate." << endl;
    exit(EXIT_FAILURE);
  }

  string outputFileName;
  cout << "Enter the name for the output file: " << endl;;
  cin >> outputFileName;
  cout << endl;
  ofstream processRun;
  processRun.open(outputFileName.c_str());
  if(!processRun.is_open()){
    cout << "Could not open process run order. Program will terminate." << endl;
    exit(EXIT_FAILURE);
  }

  vector<process> processDirectory;
  process additiveProcess;
  while(processQueue >> additiveProcess.label >> additiveProcess.arrivalTime >> additiveProcess.length){
    additiveProcess.timeRemaining = additiveProcess.length; // only used for Round Robin algorithm
    processDirectory.push_back(additiveProcess);
  }

  processRun << "** PROCESS INPUT **" << endl;
  for(int i = 0; i < processDirectory.size(); i++){
    processRun << processDirectory[i].label << " " << processDirectory[i].arrivalTime << " " << processDirectory[i].length << endl;
  }

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
  }

  sortProcessDirectory(processDirectory);

  processRun << endl << endl << "** PROCESS ARRIVAL ORDER **" << endl;
  processRun << "         process: ";
  for(int i = 0; i < processDirectory.size(); i++){
    processRun << left << setw(6) << processDirectory[i].label;
  }

  float totalExecutionTime = 0;
  for(int i = 0; i < processDirectory.size(); i++){
    totalExecutionTime += processDirectory[i].length;
  }
  float processNumber = processDirectory.size();

  vector<timelineNode> FCFSTimeline;
  float FCFSWaitingTime = 0;
  float FCFSAverageWaitingTime;
  float FCFSAverageTurnaroundTime;
  getFCFSTimeline(processDirectory, FCFSTimeline, FCFSWaitingTime);

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
  FCFSAverageWaitingTime = FCFSWaitingTime/processNumber;
  processRun << endl << "   ~waiting time: " << FCFSAverageWaitingTime;
  FCFSAverageTurnaroundTime = (FCFSWaitingTime + totalExecutionTime)/processNumber;
  processRun << endl << "~turnaround time: " << FCFSAverageTurnaroundTime;

  vector<timelineNode> SJFTimeline;
  float SJFWaitingTime = 0;
  float SJFAverageWaitingTime;
  float SJFAverageTurnaroundTime;
  getSJFTimeline(processDirectory, SJFTimeline, SJFWaitingTime);

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
  SJFAverageWaitingTime = SJFWaitingTime/processNumber;
  processRun << endl << "   ~waiting time: " << SJFAverageWaitingTime;
  SJFAverageTurnaroundTime = (SJFWaitingTime + totalExecutionTime)/processNumber;
  processRun << endl << "~turnaround time: " << SJFAverageTurnaroundTime;

  vector<timelineNode> RRTimeline;
  float RRWaitingTime = 0;
  float RRAverageWaitingTime;
  float RRAverageTurnaroundTime;
  getRRTimeline(processDirectory, TIME_QUANTUM, RRTimeline, RRWaitingTime);

  processRun << endl << endl << "** ROUND ROBIN EXECUTION TIMELINE **" << endl;
  processRun << "        process: ";
  for(int i = 0; i < RRTimeline.size(); i++){
    processRun << left << setw(6) << RRTimeline[i].label;
  }
  processRun << endl;
  processRun << "           time: ";
  for(int i = 0; i < RRTimeline.size(); i++){
    processRun << left << setw(6) << RRTimeline[i].startAtTime;
  }
  RRAverageWaitingTime = RRWaitingTime/processNumber;
  processRun << endl << "   ~waiting time: " << RRAverageWaitingTime;
  RRAverageTurnaroundTime = (RRWaitingTime + totalExecutionTime)/processNumber;
  processRun << endl << "~turnaround time: " << RRAverageTurnaroundTime;

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
        waitingTime += (cumulativeTime - sortedProcesses[0].arrivalTime);
        if(sortedProcesses[i].timeRemaining >= timeQuantum){
          additiveNode.finishAtTime = cumulativeTime + timeQuantum; //No risk in accessing non-existing elements since a NO_PROCESS_LABEL cannot occur as the last 'process'
          cumulativeTime += timeQuantum;
          sortedProcesses[i].timeRemaining -= timeQuantum;
        }
        else{
          additiveNode.finishAtTime = cumulativeTime + sortedProcesses[i].timeRemaining;
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
