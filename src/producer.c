/* test machine: csel-kh1250-01.cselabs.umn.edu 
 * group number: G[27] 
 * name: Reed Fazenbaker, Mikkel Folting
 * x500: fazen007, folti002
*/

#include "producer.h"

/**
 *
 * Producer thread will read from the file and write data to 
 * the end of the shared queue
 */
void *producer(void *arg){
  // TODO: Open the file and read its content line by line

  // Get file to read and open it
  char* inputFile = (char*) arg;
  FILE* fp = fopen(inputFile, "r");
  if(fp == NULL){
    fprintf(stderr, "Failed to open file in producer: %s\n", inputFile);
  }

  // Send data to the shared queue
  // When reaching the end of the file, send EOF message
  
  // Cleanup and exit
  return NULL; 
}
