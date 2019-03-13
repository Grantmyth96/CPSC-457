/**********************************************
 * Last Name:   DE REISET
 * First Name:  JUAN LUIS
 * Student ID:  300050167
 * Course:      CPSC 457
 * Tutorial:    T01
 * Assignment:  3
 * Question:    Q7
 *
 * 
 * File name: scheduler.cpp
 *********************************************/
 
 //For assignment instructions the executable of the file is './scheduler'
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <string>
#include <sys/stat.h>
#include <map>
#include <cstring>
#include <cstdio>
#include <fstream>
#include <queue>

using namespace std;

#define MAX_PROCESSES 30
queue<int> readyQueue;
int numberOfProcess = 0;
int totalBurstTime = 0;   // total CPU running time
int statusCounter;		  
int tQuantum;
int currentProcess = -1;    // the process is running
int countWait[MAX_PROCESSES];
int processTable [MAX_PROCESSES][2];   // 2D-array for the process table

//Initialize all possible processes statuses as empty/non-present
char status[MAX_PROCESSES] = {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '};

// The function readFile opens the give file and read it's data
// Functionalities:
//  1. calculate the total CPU burst time
//  2. insert the data in the given file into the 2D-array
//  3. return the total number of processes

int readFile(int* processT, char* ar, int* coun){
  ifstream read;   // declare an input file stream
  read.open(ar);   // open the file stream using the given file name
  
  
  if(!read){    // if the file doesn't exist, Print the error message and exit execution
    cout << "File not found " << ar << endl;
    exit(-1);
  }
  int index = 0;    // integer variable used to allocate the index in the array
  int next = 0;   // integer value that represents the result in the file
  //int numberOfProcess = 0;
  while(read >> next){   // Read from the stream in the same way as cin
    *(processT + index) = next;    // insert the read result into the array
    index++;
    if(index%2 == 0){   // if the pointer has reached the end of the line,...
      *coun = *coun + next;   // calculate the total CPU running time by adding the last result in each line(burst time)
      numberOfProcess++;	  // increases the counter for the number of processes that will be runned
    }
  }
  read.close();   // close the input stream
  return numberOfProcess;
}

// Function that prints header
void printHeader(int processNum){
  printf("Time");
  for(int i = 0; i < processNum; i++){
    cout << "\tP" << i;
  }
  printf("\n");
  printf("------------------------------------------------------------\n");
}

// Function that prints the body
void printBody(int t){
  printf("%d", t);
  for(int i=0; i<numberOfProcess; i++){
      cout << "\t" << status[i];
      if(status[i]=='+'){   // if status is 'READY' add 1 cycle to waiting time
        countWait[i]++;
      }else if(status[i]=='.'){   // if process is 'running', 1 sec is passed in totalburst time
        totalBurstTime--;
      }
  }
  printf("\n");
}

// The function that prints the waited time for each process and the average
void printEnd(){
    printf("------------------------------------------------------------\n\n");
    int sum = 0;
    for(int i = 0; i < numberOfProcess; i++){
        sum = sum + countWait[i];   // sum up the waiting time for calculating the average wait time
        cout << "P"<< i << "\twaited " << countWait[i] << " sec.\n";
    }
    printf("Average waiting time = ");
    float average = ((float) sum / numberOfProcess);   // calculate the average waiting time
    printf("%.3f sec.\n", average);
}

// Main Body
int main(int argc, char *argv[]){
  // handles command line arguments
  if(argc != 4 && argc != 3){
    fprintf(stderr, "<<Program only accepts 3 arguments>> \nUsage: ./scheduler <config_file> <'SJF' or 'RR'> <time_quantum 'in case of RR'>\n");
    exit(-1);
  }
  else if(strcmp(argv[2], "SJF") != 0 && strcmp(argv[2], "RR") != 0){
    fprintf(stderr, "<<Please enter 'RR' or 'SJF'>> \n\n");
    exit(-1);
  }
  else if(strcmp(argv[2], "RR") == 0 && argc == 3){
    fprintf(stderr, "<<RR has no quantum input>> \n\n");
    exit(-1);
  }
  else if(strcmp(argv[2], "SJF") == 0 && argc == 4){
    fprintf(stderr, "<<SJF requires no quantum input>> \n\n");
    exit(-1);
  }
  // Round-Robin Algorithm
  else if(strcmp(argv[2], "RR") == 0){   
    tQuantum = stoi(argv[3]);    
    int timeCounter = tQuantum;    // assign the time quantum to a temporal variable
    numberOfProcess = readFile((int *)processTable, argv[1], &totalBurstTime);    // read the file
    printHeader(numberOfProcess);   //print out the graph's header
    for(int i = 0; totalBurstTime > 0; i++){    // for loop that analizes the processes
        while(statusCounter < numberOfProcess){
            if(processTable[statusCounter][0] == i){    // if found a process arrived, record it and push the process to the ready queue
                readyQueue.push(statusCounter);
                status[statusCounter] = '+';
            }
            if(processTable[statusCounter][0]>i){
              break;
            }
            statusCounter++;
        }
        if(currentProcess == -1){
            if(readyQueue.size()==0){   // when where is no processes in the running state, print
                printBody(i);
            }
            else{    // if the there is process ready, pop it from the queue and put it in running state
                currentProcess = readyQueue.front();
                readyQueue.pop();
                timeCounter = tQuantum;
                processTable[currentProcess][1]--;
                status[currentProcess] = '.';
                timeCounter--;
            }
        } 
        else {
            // when there is running process
            if(timeCounter == 0){   // when the time slice finishes
                if(processTable[currentProcess][1] == 0){
                    status[currentProcess] = ' ';
                    currentProcess = -1;
                    if(readyQueue.size() == 0){   // when where is no processes in the running state, print
                        printBody(i);
                    }
                    else {   // if the there is process ready, pop it from the queue and put it in running state
                        currentProcess = readyQueue.front();
                        readyQueue.pop();
                        timeCounter = tQuantum;
                        processTable[currentProcess][1]--;
                        status[currentProcess] = '.';
                        timeCounter--;
                    }
                }
                else{	// Same as previous operation except there is an existing process
                    status[currentProcess] = '+';
                    readyQueue.push(currentProcess);
                    currentProcess = readyQueue.front();
                    readyQueue.pop();
                    timeCounter = tQuantum;
                    processTable[currentProcess][1]--;
                    status[currentProcess] = '.';
                    timeCounter--;
                }
            }
            else{    // when the time slice is not finish
                if(processTable[currentProcess][1] == 0){
                    status[currentProcess] = ' ';
                    currentProcess = -1;
                    if(readyQueue.size()==0){   // when where is no processes in the running state, print body
                        printBody(i);
                    }
                    else {
                        // if the there is process ready, pop it from the queue and put it in running state
                        currentProcess = readyQueue.front();
                        readyQueue.pop();
                        timeCounter = tQuantum;
                        processTable[currentProcess][1]--;
                        status[currentProcess] = '.';
                        timeCounter--;
                    }
                
                }
                else{
					// subcase in whcih the process is not finished
                    processTable[currentProcess][1]--;
                    timeCounter--;
                }
            }
        }
        // after the time i, prints te result in the graph body
        printBody(i);
    }
    // After the main body, prints the waited time for each process and calculate the average wait time
    printEnd();
  }
  
  // Shortest-job-first ALgorithm
  else if(strcmp(argv[2], "SJF") == 0){
    numberOfProcess = readFile((int *)processTable, argv[1], &totalBurstTime);    // read the file
    printf("Process#: %d, Total burst: %d\n", numberOfProcess, totalBurstTime);
    printHeader(numberOfProcess);   //print out the graph's header
    for(int i = 0; totalBurstTime > 0; i++){
        while(statusCounter < numberOfProcess){
            if(processTable[statusCounter][0] == i){    // if found a process arrived, record it
                status[statusCounter] = '+';
            }
            else if(processTable[statusCounter][0] > i){
              break;
            }
            statusCounter++;
        }
        if(currentProcess == -1){
            // Finding the the ready queue that arrived first
            int min = -1;
            for(int i = 0 ; i < statusCounter; i++){
                if(min == -1 && status[i] == '+'){
                  min = i;    // if no process is found, then set the readyed process to be the first process
                }
                else if(processTable[i][1] < processTable[min][1] && status[i] == '+'){
                  min = i;    // if the there is processes arrived earlier then the recorded process and it is readyed,the nset that process to be the first process
                }
            }
            if(min != -1){    // case in which the first process has been found
                currentProcess = min;   // assign the currect process
                status[currentProcess] = '.';
                processTable[currentProcess][1]--;
            }
        }
        else if(processTable[currentProcess][1] == 0){   // the case when the process is finished
            status[currentProcess] = ' ';
            currentProcess = -1;
            int min = -1;
            for (int i = 0 ; i < statusCounter; i++){
                if(min == -1 && status[i]=='+'){
                  min = i;
                }else if(processTable[i][1] < processTable[min][1] && status[i] == '+'){
                  min = i;
                }
            }
            if(min!=-1){
                currentProcess = min;
                status[currentProcess] = '.';
                processTable[currentProcess][1]--;
            }
        }
        else{
            processTable[currentProcess][1]--;
        }
        // after the time i, prints te result in the graph body
        printBody(i);
    }
    // After the main body, prints the waited time for each process and calculate the average wait time
    printEnd();
  }
  return 0;
}
