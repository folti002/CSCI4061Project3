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
	char* emptyString;
	// Get customer ID
	int id = atoi(strtok_r(line, ",", &emptyString));

	// Sum up transactions for the current customer
	double balanceChange = 0.0;
	char* token = strtok_r(NULL, ",", &emptyString);
	while(token != NULL){
		balanceChange += strtod(token, NULL);
		token = strtok_r(NULL, ",", &emptyString);
	}
	//printf("id %d: %lf\n", id, balanceChange);
	fflush(stdout);

	// Update the global array
	pthread_mutex_lock(&balanceLock);
	balance[id] += balanceChange;
	pthread_mutex_unlock(&balanceLock);
}

void *consumer(void *arg){
	int consumerID = *(int*) arg;
	if(runOption == 1 || runOption == 3){
		fprintf(logFile, "consumer %d\n", consumerID);
    fflush(logFile);
	}
	
	// Keep reading from queue and process the data
	int counter = 0;
	while(true){

		//printf("%d %s", head->eof, head->transactions);
		// while(head == tail){
		// 	sem_wait(&bufferSem);
		// }

		sem_wait(&mut);
		if(head == tail){
			printf("we should be at the end now\n");
			break;
		}

		counter++;
		if(head->eof == 0){
			//printf("Node with data.\n");
			//fflush(stdout);
			if(runOption == 1 || runOption == 3){
				fprintf(logFile, "consumer %d: line %d\n", consumerID, head->lineNumber);
				fflush(logFile);
			}
			char *copiedStr = (char *)malloc(sizeof(char) * chunkSize);
			strcpy(copiedStr, head->transactions);
			// printf("%s", copiedStr);
			parse(copiedStr);
			free(copiedStr);
		}	else {
			if(runOption == 1 || runOption == 3){
				fprintf(logFile, "consumer %d: line -1\n", consumerID);
				fflush(logFile);
			}
			//printf("EOF reached\n");
			//fflush(stdout);
			packet* behind = head;
			head = head->next;
			free(behind->transactions);
			free(behind);
			sem_post(&mut);
			break;
		}
		packet* behind = head;
		head = head->next;
		free(behind->transactions);
		free(behind);
		//sem_post(&slots);
		sem_post(&mut);
	}
	
	return NULL; 
}