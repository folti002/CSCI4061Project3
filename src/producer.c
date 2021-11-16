/* test machine: csel-kh1250-01.cselabs.umn.edu 
 * group number: G[27] 
 * name: Reed Fazenbaker, Mikkel Folting
 * x500: fazen007, folti002
*/

#include "producer.h"

packet* tail;
packet* head;

int numConsumers;
FILE* logFile;
pthread_mutex_t sharedQueueLock;
pthread_cond_t cond;

sem_t bufferSem;
sem_t mut;
sem_t slots;

// Helper method to check if linked list holds the correct information
void printLinkedList(packet* startNode){
  packet* temp = startNode;
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
  int counter = 0;
  while(getLineFromFile(fp, curLine, chunkSize) != -1){
    packet* newNode = (packet*) malloc(sizeof(packet));
    newNode->transactions = (char*) malloc(sizeof(char) * chunkSize);
    strcpy(newNode->transactions, curLine);
    newNode->next = NULL;
    newNode->eof = 0;
    newNode->lineNumber = counter;
    
    //sem_wait(&slots);
    sem_wait(&mut);
    if(head == NULL){
      head = newNode;
      tail = newNode;
      sem_post(&bufferSem);
    } else {
      tail->next = newNode;
      tail = newNode;
      //sem_post(&bufferSem);
    }
    sem_post(&mut);

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

  // Send nodes indicating an end of file into the queue
  for(int i = 0; i < numConsumers; i++){
    packet* eofNode = (packet*) malloc(sizeof(packet));
    eofNode->eof = 1;
    eofNode->next = NULL;
    eofNode->transactions = (char*) malloc(chunkSize);
    strcpy(eofNode->transactions, "eof node\n");

    sem_wait(&mut);
    if(head == NULL){
      head = eofNode;
      tail = eofNode;
      sem_post(&bufferSem);
    } else {
      tail->next = eofNode;
      tail = eofNode;
      sem_post(&bufferSem);
    }
    sem_post(&mut);
    //sem_wait(&slots);
  }
  tail = NULL;

  //printLinkedList(tail);

  // Cleanup and exit
  fclose(fp);
  return NULL; 
}