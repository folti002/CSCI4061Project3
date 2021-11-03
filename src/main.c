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

	bookeepingCode();
	
	// TODO: Initialize global variables, like shared queue
	
	// TODO: Create producer and consumer threads

	// TODO: Wait for all threads to complete execution
	
	// Write the final output
	writeBalanceToFiles();
	
	return 0; 
}