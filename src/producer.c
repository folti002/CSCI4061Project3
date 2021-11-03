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

  // Get file to read from arg passed into this method
  char* inputFile = (char*) arg;

  // Send data to the shared queue
  // When reaching the end of the file, send EOF message
  
  // Cleanup and exit
  return NULL; 
}
