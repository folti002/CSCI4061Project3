/* test machine: csel-kh1250-01.cselabs.umn.edu 
 * group number: G[27] 
 * name: Reed Fazenbaker, Mikkel Folting
 * x500: fazen007, folti002
*/

#include "producer.h"

FILE* logFile;
pthread_mutex_t sharedQueueLock;
pthread_cond_t cond;

// Helper method to check if linked list holds the correct information
void printLinkedList(packet* head){
  packet* temp = head->next;
  while(temp != NULL){
    printf("%s", temp->transactions);
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
  int counter = 0;
  while(getLineFromFile(fp, curLine, chunkSize) != -1){
    packet* newNode = (packet*) malloc(sizeof(packet));
    newNode->transactions = (char*) malloc(chunkSize);
    strcpy(newNode->transactions, curLine);
    newNode->next = NULL;
    newNode->eof = 0;
    newNode->lineNumber = counter;
    
    temp->next = newNode;
    sem_post(&sem);

    temp = newNode;

    // Write progress of producer to log file
    if(runOption == 1 || runOption == 3){
		  fprintf(logFile, "producer: line %d\n", counter);
      fflush(logFile);
	  }
    counter++;
  }

  // Send EOF message
  if(runOption == 1 || runOption == 3){
    fprintf(logFile, "producer: line -1\n");
    fflush(logFile);
  }
  packet* eofNode = (packet*) malloc(sizeof(packet));
  eofNode->eof = 1;
  eofNode->next = NULL;
  eofNode->transactions = NULL;
  temp->next = eofNode;

  //printLinkedList(head);
  
  // Cleanup and exit
  fclose(fp);
  return NULL; 
}