/* test machine: csel-kh1250-01.cselabs.umn.edu 
 * group number: G[27] 
 * name: Reed Fazenbaker, Mikkel Folting
 * x500: fazen007, folti002
*/

#include "producer.h"

FILE* logFile;

void printLinkedList(packet* head){
  packet* temp = head->next;
  int counter = 1;
  while(temp != NULL){
    printf("%s", temp->transactions);
    counter++;
    temp = temp->next;
  }
}

/**
 * Producer thread will read from the file and write data to 
 * the end of the shared queue
 */
void *producer(void *arg){
  // Write to log file
  if(runOption == 1 || runOption == 3){
		fprintf(logFile, "producer\n");
    fflush(logFile);
	}

  // Get file to read and open it
  char* inputFile = (char*) arg;
  FILE* fp = fopen(inputFile, "r");
  if(fp == NULL){
    fprintf(stderr, "Failed to open file in producer: %s\n", inputFile);
  }

  // Read file line by line and send data to the shared queue
  char curLine[chunkSize];

  packet* temp = head;
  //printf("mem address of head in producer: %p\n", temp);
  int counter = 0;
  while(getLineFromFile(fp, curLine, chunkSize) != -1){
    packet* newNode = (packet*) malloc(sizeof(packet));
    newNode->transactions = (char*) malloc(chunkSize);
    strcpy(newNode->transactions, curLine);
    newNode->next = NULL;
    temp->next = newNode;
    temp = newNode;

    // Write to log file
    if(runOption == 1 || runOption == 3){
		  fprintf(logFile, "producer: line %d\n", counter);
      fflush(logFile);
	  }
    counter++;
  }

  printLinkedList(head);

  // Send data to the shared queue
  // When reaching the end of the file, send EOF message
  
  // Cleanup and exit
  return NULL; 
}