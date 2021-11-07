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
FILE* logFile;

pthread_mutex_t sharedQueueLock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

/**
 * Write final balance to a single file.
 * The path name should be output/result.txt
 */
void writeBalanceToFiles(void) {
	// Write balance for each customer to output file
	FILE* results = fopen("output/result.txt", "w");
	double assetsChange = 0.0;
	for(int i = 0; i < acctsNum; i++){
		fprintf(results, "%d\t%lf\n", i, balance[i]);
		assetsChange += balance[i];
	}

	// Write total balance change and close file
	fprintf(results, "All:\t%lf\n", assetsChange);
	fclose(results);
}

int main(int argc, char *argv[]){
	bookeepingCode();

	// If the argument count isn't between 3 and 5, let user know and exit the program
	if(argc < 3 || argc > 5){
		fprintf(stderr, "Usage ./bank [number of consumers] [input file]\nOR\nUsage ./bank [number of consmers] [inputFile] [option] [queue size]\n");
		exit(EXIT_FAILURE);
	}

	// Store number of consumers and the input file to use
	int numConsumers = atoi(argv[1]);
	char* inputFile = argv[2];

	// Set run option to the approprate value based on argument inputted
	// Additionally, store the inputted queue size in the extern variable if specified
	if(argc == 4){
		char* option = argv[3];
		if(strcmp(option, "-p") == 0){
			runOption = 1;
			logFile = fopen("output/log.txt", "w");
		} else {
			fprintf(stderr, "ERROR: Invalid option inputted\n");
			exit(EXIT_FAILURE);
		}
	} else if(argc == 5){
		char* option = argv[3];
		if(strcmp(option, "-b") == 0){
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

	// Initialize shared data queue
	head = (packet*) malloc(sizeof(packet));
	head->next = NULL;

	// Initialize balance array to all zeroes
	for(int i = 0; i < acctsNum; i++){
		balance[i] = 0;
	}
	
	// Create producer thread and write to log file if necessary
	pthread_t producer_tid;
	if(pthread_create(&producer_tid, NULL, producer, (void*) inputFile) != 0){
		printf("Producer thread failed to create\n");
	}

	// Initialize consumer thread IDs and array of consumer IDs
	pthread_t* consumer_tids = (pthread_t*) malloc(sizeof(pthread_t) *numConsumers);
	int* consumerIDs = (int*) malloc(sizeof(int) * numConsumers);
	for(int i = 0; i < numConsumers; i++){
		consumerIDs[i] = i;
	}

	// Create consumer threads
	for(int i = 0; i < numConsumers; i++){
		if(pthread_create(&(consumer_tids[i]), NULL, consumer, (void*) &consumerIDs[i]) != 0){
			printf("Consumer thread %d failed to create\n", i);
		}
	}

	// Wait for producer thread to complete
	pthread_join(producer_tid, NULL);

	// Wait for consumer threads to complete
	for(int i = 0; i < numConsumers; i++) {\
		pthread_join(consumer_tids[i], NULL);
	}
	
	// Write the final output
	writeBalanceToFiles();

	// Free malloc'd variables and close files
	free(consumer_tids);
	if(runOption == 1 || runOption == 3){
		fclose(logFile);
	}

	// Free shared queue
	packet* temp = head->next;
	packet* behind = head;
	while(temp != NULL){
		temp = temp->next;
		free(behind->transactions);
		free(behind);
		behind = temp;
	}
	free(behind);
	
	return 0; 
}