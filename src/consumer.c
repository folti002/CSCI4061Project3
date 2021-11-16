/* test machine: csel-kh1250-01.cselabs.umn.edu 
 * group number: G[27] 
 * name: Reed Fazenbaker, Mikkel Folting
 * x500: fazen007, folti002
*/

#include "consumer.h"
#include <ctype.h>

FILE* logFile;
pthread_mutex_t sharedQueueLock;
pthread_cond_t cond;
pthread_mutex_t balanceLock;

packet* tail;
packet* head;

sem_t bufferSem;
sem_t mut;
sem_t slots;

/**
 * Parse lines from the queue, calculate balance change
 * and update to global array
 */
void parse(char *line){
	// Get customer ID
	char* emptyString;
	int id = atoi(strtok_r(line, ",", &emptyString));

	// Sum up transactions for the current customer
	double balanceChange = 0.0;
	char* token = strtok_r(NULL, ",", &emptyString);
	while(token != NULL){
		balanceChange += strtod(token, NULL);
		token = strtok_r(NULL, ",", &emptyString);
	}

	// Update the global array
	pthread_mutex_lock(&balanceLock);
	balance[id] += balanceChange;
	pthread_mutex_unlock(&balanceLock);
}

void *consumer(void *arg){
	// Take in the ID of this specific consumer from input to function and write to log file
	int consumerID = *(int*) arg;
	if(runOption == 1 || runOption == 3){
		fprintf(logFile, "consumer %d\n", consumerID);
    fflush(logFile);
	}
	
	// Keep reading from queue and process the data
	int counter = 0;
	while(true){
		// Grab the mutex but release it if head is still null
		// Once head isn't null anymore, grab it again and check the condition to make sure
		sem_wait(&mut);
		while(head == NULL){
			sem_post(&mut);
			sem_wait(&bufferSem);
			sem_wait(&mut);
		}

		// Check if we are dealing with an eof packet or not
		if(head->eof == 0){
			// If the current packet isn't an eof packet, call parse on its transactions and write to log file
			if(runOption == 1 || runOption == 3){
				fprintf(logFile, "consumer %d: line %d\n", consumerID, head->lineNumber);
				fflush(logFile);
			}
			char* copiedStr = (char*) malloc(sizeof(char) * chunkSize);
			strcpy(copiedStr, head->transactions);
			parse(copiedStr);
			free(copiedStr);

			// Let producer know that another slot has opened up in the queue
			if(runOption == 2 || runOption == 3){
				sem_post(&slots);
			}
		}	else {
			// If the current packet is an eof packet, free it and then break from the while loop
			if(runOption == 1 || runOption == 3){
				fprintf(logFile, "consumer %d: line -1\n", consumerID);
				fflush(logFile);
			}
			packet* behind = head;
			head = head->next;
			free(behind->transactions);
			free(behind);

			// Let producer know that another slot has opened up in the queue
			if(runOption == 2 || runOption == 3){
				sem_post(&slots);
			}
			sem_post(&mut);
			break;
		}

		// Free the packet we just parsed and give up the mutex lock
		packet* behind = head;
		head = head->next;
		free(behind->transactions);
		free(behind);
		sem_post(&mut);
	}
	
	return NULL; 
}