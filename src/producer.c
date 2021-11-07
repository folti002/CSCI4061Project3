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
    
    // Lock before directly accessing the shared queue
    pthread_mutex_lock(&sharedQueueLock);
    temp->next = newNode;
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&sharedQueueLock);

    temp = newNode;

    // Write progress of producer to log file
    if(runOption == 1 || runOption == 3){
		  fprintf(logFile, "producer: line %d\n", counter);
      fflush(logFile);
	  }
    counter++;
  }

  // SEND EOF MESSAGE



  //printLinkedList(head);
  
  // Cleanup and exit
  fclose(fp);
  return NULL; 
}