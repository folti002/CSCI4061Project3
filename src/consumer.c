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

/**
 * Parse lines from the queue, calculate balance change
 * and update to global array
 */
void parse(char *line){
	// Get customer ID
	int id = atoi(strtok(line, ","));
	printf("id %d\n", id);

	// Sum up transactions for the current customer
	double balanceChange = 0.0;
	char* token = strtok(NULL, ",");
	while(token != NULL){
		balanceChange += strtod(token, NULL);
		token = strtok(NULL, ",");
	}
	printf("id %d: %lf\n", id, balanceChange);

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
	
	// TODO: Keep reading from queue and process the data
	// pthread_mutex_lock(&sharedQueueLock);
	// packet* temp = head->next;
	// while(temp == NULL){
	// 	pthread_cond_wait(&cond, &sharedQueueLock);
	// }
	// parse(temp->transactions);
	// pthread_mutex_unlock(&sharedQueueLock);

	// With semaphores
	sem_wait(&sem);
	packet* temp = head->next;
	while(temp != NULL){
		if(temp->eof != 1){
			if(runOption == 1 || runOption == 3){
				fprintf(logFile, "consumer %d: line %d\n", consumerID, temp->lineNumber);
				fflush(logFile);
			}
			parse(temp->transactions);
		}	else {
			if(runOption == 1 || runOption == 3){
				fprintf(logFile, "consumer %d: line -1\n", consumerID);
				fflush(logFile);
			}
			printf("EOF reached\n");
			break;
		}
		temp = temp->next;
		head = temp;
	}
	
	return NULL; 
}