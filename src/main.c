/* test machine: csel-kh1250-01.cselabs.umn.edu 
 * group number: G[27] 
 * name: Reed Fazenbaker, Mikkel Folting
 * x500: fazen007, folti002
*/

#include "header.h"

packet* head;
int runOption;
int queueSize;
double balance[acctsNum];

/**
 * Write final balance to a single file.
 * The path name should be output/result.txt
 */
void writeBalanceToFiles(void) {
	// TODO: Write balance for each customer 
	

	// TODO: Write total balance change

}

int main(int argc, char *argv[]){
	bookeepingCode();

	// If the argument count isn't between 3 and 5, exit the program
	if(argc < 3 || argc > 5){
		fprintf(stderr, "Usage ./bank [number of consumers] [input file]\nOR\nUsage ./bank [number of consmers] [inputFile] [option] [queue size]\n");
		exit(EXIT_FAILURE);
	}
	// Store number of consumers and the input file to use
	int numConsumers = atoi(argv[1]);
	char* inputFile = argv[2];

	// Set run option to the approprate value based on argument inputted
	// Additionally, store the inputted queue size in the extern variable
	if(argc == 5){
		char* option = argv[3];
		if(strcmp(option, "-p") == 0){
			runOption = 1;
			FILE* fp = fopen("output/log.txt", "w");
		} else if(strcmp(option, "-b") == 0){
			runOption = 2;
			queueSize = atoi(argv[4]);
		} else if(strcmp(option, "-bp") == 0){
			runOption = 3;
			queueSize = atoi(argv[4]);
		} else {
			fprintf(stderr, "ERROR: Invalid option inputted\n");
			exit(EXIT_FAILURE);
		}
	} else {
		runOption = 0;
	}
	
	// TODO: Initialize global variables, like shared queue

	// Initialize shared data queue
	head = (packet*) malloc(sizeof(packet));
	head->next = NULL;

	// Initialize balance array to all zeroes
	for(int i = 0; i < acctsNum; i++){
		balance[i] = 0;
	}
	
	// Create producer thread
	pthread_t producer_tid;
	if(pthread_create(&producer_tid, NULL, producer, (void*) inputFile) != 0){
		printf("Producer thread failed to create\n");
	}
	printf("Producer thread created successfully\n");

	// Create consumer threads
	pthread_t* consumer_tids = (pthread_t*) malloc(sizeof(pthread_t) *numConsumers);
	for(int i = 0; i < numConsumers; i++){
		consumer_tids[i] = i;
	}

	for(int i = 0; i < numConsumers; i++){
		if(pthread_create(&(consumer_tids[i]), NULL, consumer, (void*) &consumer_tids[i]) != 0){
			printf("Consumer thread %d failed to create\n", i);
		}
		//printf("Consumer thread %d created successfully\n", i);
	}

	// Wait for producer thread to complete
	pthread_join(producer_tid, NULL);

	// Wait for consumer threads to complete
	for(int i = 0; i < numConsumers; i++) {
		//pthread_join(consumer_tids[i], NULL);
		pthread_join(consumer_tids[i], NULL);
	}
	
	// Write the final output
	writeBalanceToFiles();

	// Free malloc'd variables
	free(consumer_tids);

	// Free shared queue
	packet* temp = head->next;
	packet* behind = head;
	while(temp != NULL){
		temp = temp->next;
		free(behind);
		behind = temp;
	}
	
	return 0; 
}