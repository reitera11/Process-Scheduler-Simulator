#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>

#define NO_PROCESS_LABEL "NONE" //The literal used as a label to describe the state when no process is executing

using namespace std;

struct process{
  string label;
  int arrivalTime;
  int length;
};

struct timelineNode{
  string label;
  int startAtTime;
  int finishAtTime;
};

void sortProcessDirectory(vector<process>& unsortedProcesses);
void getFCFSTimeline(const vector<process>& sortedProcesses, vector<timelineNode>& timeline);
string getCurrentProcess(const vector<timelineNode>& timeline, int enquiryTime);
void getSJFTimeline(const vector<process>& sortedProcesses, vector<timelineNode>& timeline);

int main(){

  ifstream processQueue;
  processQueue.open("inputProcesses.txt");
  if(!processQueue.is_open()){
    cout << "Could not open process queue. Program will terminate.";
    exit(EXIT_FAILURE);
  }

  ofstream processRun;
  processRun.open("outputProcesses.txt");
  if(!processRun.is_open()){
    cout << "Could not open process run order. Program will terminate.";
    exit(EXIT_FAILURE);
  }

  vector<process> processDirectory;
  process additiveProcess;
  while(processQueue >> additiveProcess.label >> additiveProcess.arrivalTime >> additiveProcess.length){
    processDirectory.push_back(additiveProcess);
  }

  sortProcessDirectory(processDirectory);

  processRun << "** PROCESS DETAILS **" << endl;
  for(int i = 0; i < processDirectory.size(); i++){
    processRun << processDirectory[i].label << " " << processDirectory[i].arrivalTime << " " << processDirectory[i].length << endl;
  }

  processRun << endl << "** PROCESS ARRIVAL ORDER **" << endl;
  for(int i = 0; i < processDirectory.size(); i++){
    processRun << processDirectory[i].label << " ";
  }

  vector<timelineNode> FCFSTimeline;
  getFCFSTimeline(processDirectory, FCFSTimeline);

  vector<timelineNode> SJFTimeline;
  getSJFTimeline(processDirectory, SJFTimeline);


  processRun << endl << endl << "** FCFS EXECUTION TIMELINE **" << endl;
  processRun << "process: ";
  for(int i = 0; i < FCFSTimeline.size(); i++){
    processRun << left << setw(6) << FCFSTimeline[i].label;
  }
  processRun << endl;
  processRun << "time:    ";
  for(int i = 0; i < FCFSTimeline.size(); i++){
    processRun << left << setw(6) << FCFSTimeline[i].startAtTime;
  }

  processRun << endl << endl << "** SJF EXECUTION TIMELINE **" << endl;
  processRun << "process: ";
  for(int i = 0; i < SJFTimeline.size(); i++){
    processRun << left << setw(6) << SJFTimeline[i].label;
  }
  processRun << endl;
  processRun << "time:    ";
  for(int i = 0; i < SJFTimeline.size(); i++){
    processRun << left << setw(6) << SJFTimeline[i].startAtTime;
  }

  processQueue.close();
  processRun.close(); //close files before the end of the prgram so console features do not halt output file creation

  int enquiryTime;
  cout << endl << "t = ";
  cin >> enquiryTime;
  string processAtEnquiryTime = getCurrentProcess(FCFSTimeline, enquiryTime);
  cout << endl << "At time t = " << enquiryTime << " the process with label " << processAtEnquiryTime << " was executing." << endl;
  cout << endl << "Note: a process with label" << NO_PROCESS_LABEL << " indicates no process was executing at the enquiry time." << endl;

return 0;
}

void sortProcessDirectory(vector<process>& unsortedProcesses){
  for(int j = 0; j < unsortedProcesses.size() - 1; j++){
    bool didSwap = false;
    for(int i = 1; i < unsortedProcesses.size(); i++){
      if(unsortedProcesses[i-1].arrivalTime > unsortedProcesses[i].arrivalTime){
        process tmpProcess = unsortedProcesses[i-1];
        unsortedProcesses[i-1] = unsortedProcesses[i];
        unsortedProcesses[i] = tmpProcess;
        didSwap = true;
      }
    }
    if(!didSwap){
      j = unsortedProcesses.size() + 1;
    }
  }
}

void getFCFSTimeline(const vector<process>& sortedProcesses, vector<timelineNode>& timeline){
  int cumulativeTime = 0;
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
    cumulativeTime += sortedProcesses[i].length;
  }
  additiveNode.label = "END";
  additiveNode.startAtTime = cumulativeTime;
  additiveNode.finishAtTime = cumulativeTime;
  timeline.push_back(additiveNode);
}

string getCurrentProcess(const vector<timelineNode>& timeline, int enquiryTime){
  string currentProcessLabel;
  bool currentProcessFound = false;
  int i = 0;
  while(currentProcessFound == false){
    if(enquiryTime >= timeline[i].startAtTime && enquiryTime < timeline[i].finishAtTime){
      currentProcessLabel = timeline[i].label;
      currentProcessFound = true;
    }
    else{
      i++;
    }
  }
  return currentProcessLabel;
}

void getSJFTimeline(const vector<process>& sortedProcesses, vector<timelineNode>& timeline){
  int cumulativeTime = 0;
  timelineNode additiveNode;
  vector<process> waitingProcesses;
    int waitingProcessesAdded = 0;
  // bool processesAreWaiting; //add in for case when only one process is waiting. No need to sort vector etc.
  for(int i = 0; i < sortedProcesses.size(); i++){
    if(sortedProcesses[i].arrivalTime > cumulativeTime){
      additiveNode.label = NO_PROCESS_LABEL;
      additiveNode.startAtTime = cumulativeTime;
      additiveNode.finishAtTime = sortedProcesses[i+1].arrivalTime; //No risk in accessing non-existing elements since a NO_PROCESS_LABEL cannot occur as the last 'process'
      timeline.push_back(additiveNode);
      cumulativeTime = sortedProcesses[i].arrivalTime;
    }
    for(int j = i; j < sortedProcesses.size(); j++){ //need to find alternative stable way, for loop not needed since processes are already sorted
      waitingProcessesAdded = 0;
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
    cumulativeTime += waitingProcesses[0].length;
    waitingProcesses.erase(waitingProcesses.begin());

    i += (waitingProcessesAdded - 1);
  }
  if(waitingProcesses.size() != 0){
    for(int p = 0; p < waitingProcesses.size(); p++){
      additiveNode.label = waitingProcesses[p].label;
      additiveNode.startAtTime = cumulativeTime;
      additiveNode.finishAtTime = cumulativeTime + waitingProcesses[p].length;
      timeline.push_back(additiveNode);
      cumulativeTime += waitingProcesses[p].length;
    }
  }
}
