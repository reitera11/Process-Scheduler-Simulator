#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>

using namespace std;

struct process{
  string label;
  int arrivalTime;
  int length;
};

struct timelineNode{
  string label;
  int atTime;
};

void sortProcessDirectory(vector<process>& unsortedProcesses);
void getDiscreteTimeline(const vector<process>& sortedProcesses, vector<timelineNode>& timeline);

int main(){

  ifstream processQueue;
  processQueue.open("testProcess.txt");
  if(!processQueue.is_open()){
    cout << "Could not open process queue. Program will terminate.";
    exit(EXIT_FAILURE);
  }

  ofstream processRun;
  processRun.open("testRun.txt");
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

  processRun << "** RUN ORDER **" << endl;
  for(int i = 0; i < processDirectory.size(); i++){
    processRun << processDirectory[i].label << " ";
  }

  processRun << endl << endl << "** PROCESS DETAILS **" << endl;
  for(int i = 0; i < processDirectory.size(); i++){
    processRun << processDirectory[i].label << " " << processDirectory[i].arrivalTime << " " << processDirectory[i].length << endl;
  }

  vector<timelineNode> discreteTimeline;
  getDiscreteTimeline(processDirectory, discreteTimeline);

  processRun << endl << "** EXECUTION TIMELINE **" << endl;
  processRun << "process: ";
  for(int i = 0; i < discreteTimeline.size(); i++){
    processRun << left << setw(6) << discreteTimeline[i].label;
  }
  processRun << endl;
  processRun << "time:    ";
  for(int i = 0; i < discreteTimeline.size(); i++){
    processRun << left << setw(6) << discreteTimeline[i].atTime;
  }

  processQueue.close();
  processRun.close();

return 0;
}

void sortProcessDirectory(vector<process>& unsortedProcesses){
  for(int j = 0; j < unsortedProcesses.size() - 1; j++){
    for(int i = 1; i < unsortedProcesses.size(); i++){
      if(unsortedProcesses[i-1].arrivalTime > unsortedProcesses[i].arrivalTime){
        process tmpProcess = unsortedProcesses[i-1];
        unsortedProcesses[i-1] = unsortedProcesses[i];
        unsortedProcesses[i] = tmpProcess;
      }
    }
  }
}

void getDiscreteTimeline(const vector<process>& sortedProcesses, vector<timelineNode>& timeline){
  int cumulativeTime = 0;
  timelineNode additiveNode;
  for(int i = 0; i < sortedProcesses.size(); i++){
    if(sortedProcesses[i].arrivalTime > cumulativeTime){
      additiveNode.label = "NONE";
      additiveNode.atTime = cumulativeTime;
      timeline.push_back(additiveNode);
      cumulativeTime = sortedProcesses[i].arrivalTime;
    }
    additiveNode.label = sortedProcesses[i].label;
    additiveNode.atTime = cumulativeTime;
    timeline.push_back(additiveNode);
    cumulativeTime += sortedProcesses[i].length;
  }
  additiveNode.label = "END";
  additiveNode.atTime = cumulativeTime;
  timeline.push_back(additiveNode);
}
