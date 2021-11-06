/* test machine: csel-kh1250-01.cselabs.umn.edu 
 * group number: G[27] 
 * name: Reed Fazenbaker, Mikkel Folting
 * x500: fazen007, folti002
*/

#include "header.h"

/**
 * Write final balance to a single file.
 * The path name should be output/result.txt
 */
void writeBalanceToFiles(void) {
	// TODO: Write balance for each customer 
	

	// TODO: Write total balance change

}

int main(int argc, char *argv[]){
	//TODO: Argument check
	if(argc != 3){
		fprintf(stderr, "Usage ./bank [number of consumers] [input file]\n");
		exit(EXIT_FAILURE);
	}
	int numConsumers = argv[1];
	char* inputFile = argv[2];

	bookeepingCode();
	
	// TODO: Initialize global variables, like shared queue
	
	// Create producer thread
	pthread_t producer_tid;
	if(pthread_create(&producer_tid, NULL, producer, (void*) inputFile) != 0){
		printf("Producer thread failed to create\n");
	}
	printf("Producer thread created successfully\n");

	// Create consumer threads
	pthread_t consumer_tids[numConsumers];
	for(int i = 0; i < numConsumers; i++){
		if(pthread_create(&(consumer_tids[i]), NULL, consumer, NULL) != 0){
			printf("Consumer thread %d failed to create\n", i);
		}
		printf("Consumer thread %d created successfully\n", i);
	}

	// Wait for producer thread to complete
	pthread_join(producer_tid, NULL);

	// Wait for consumer threads to complete
	for(i = 0; i < numConsumers; i++) {
		pthread_join(consumer_tids[i], NULL);
	}
	
	// Write the final output
	writeBalanceToFiles();
	
	return 0; 
}