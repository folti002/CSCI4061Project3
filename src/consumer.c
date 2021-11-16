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
	int id = atoi(strtok(line, ","));

	// Sum up transactions for the current customer
	double balanceChange = 0.0;
	char* emptyString;
	char* token = strtok_r(NULL, ",", emptyString);
	while(token != NULL){
		balanceChange += strtod(token, NULL);
		token = strtok_r(NULL, ",", emptyString);
	}
	//printf("id %d: %lf\n", id, balanceChange);
	fflush(stdout);

	// Update the global array
	pthread_mutex_lock(&balanceLock);
	balance[id] = balanceChange;
	pthread_mutex_unlock(&balanceLock);
}

void *consumer(void *arg){
	int consumerID = *(int*) arg;
	if(runOption == 1 || runOption == 3){
		fprintf(logFile, "consumer %d\n", consumerID);
    fflush(logFile);
	}
	
	// Keep reading from queue and process the data
	// sem_wait(&bufferSem);
	// sem_wait(&mut);
	int counter = 0;
	while(true){
		while(head == NULL){
			sem_wait(&preventConsumerFromRunningFirst);
		}

		while(head == tail){
			sem_wait(&bufferSem);
		}
		sem_wait(&mut);


		if(tail == NULL){
			printf("HIIII %p %d\n", tail, counter);
			break;
		}

		printf("%d %s", tail->eof, tail->transactions);

		counter++;
		if(tail->eof != 1){
			//printf("Node with data.\n");
			fflush(stdout);
			if(runOption == 1 || runOption == 3){
				fprintf(logFile, "consumer %d: line %d\n", consumerID, tail->lineNumber);
				fflush(logFile);
			}
			parse(tail->transactions);
		}	else {
			if(runOption == 1 || runOption == 3){
				fprintf(logFile, "consumer %d: line -1\n", consumerID);
				fflush(logFile);
			}
			printf("EOF reached\n");
			fflush(stdout);
			sem_post(&mut);
			break;
		}
		packet* behind = tail;
		tail = tail->next;
		free(behind->transactions);
		free(behind);
		sem_post(&mut);
		//sem_post(&slots);
	}
	
	return NULL; 
}