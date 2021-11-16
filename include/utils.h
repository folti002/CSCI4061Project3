#ifndef UTILS_H
#define UTILS_H

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define chunkSize 1024
#define acctsNum 1000

/* shared array definition */
extern double balance[acctsNum];

/* file I/O */
/**
 * Get a pointer to a opened file based on the file name
 * @param *inputFileName  the file path
 * @return a file pointer pointing to the file
 */
FILE * getFilePointer(char *inputFileName);

/**
 * Read an entire line from a file
 * @param  *fp    the file to be read
 * @param  *line  contain the line content
 * @param  len    the size of the line
 * @return the number of character reads (including the newline \n, but not including terminator)
         -1 when reaching the end of file or error occurs
  */
ssize_t getLineFromFile(FILE *fp, char *line, size_t len);

/**
 * Open a file, and write a line to the file
 * @param *filepath  the file path
 * @param *line      the line content
 */
void writeLineToFile(char *filepath, char *line);

/* directory */
void bookeepingCode();

/* other function declaration */

/* packet struct definition */
typedef struct packet {
  char* transactions;
  struct packet* next;
  int lineNumber; // Number of line in input file
  int eof; // 1 if true, 0 if false
} packet;

extern packet* head;
extern packet* tail;

/* Additional global shared variables */
extern int runOption;
extern int queueSize;
extern int numConsumers;

extern FILE* logFile;

extern pthread_mutex_t sharedQueueLock;
extern pthread_cond_t cond;

extern pthread_mutex_t balanceLock; // Used to protect access to global balance array

extern sem_t bufferSem; // Used to let producer signal consumer when a line is added to the queue
extern sem_t mut; // Used for mutual exclusion when accessing shared queue
extern sem_t slots; // Used for upper bound on queue size

extern sem_t preventConsumerFromRunningFirst;

#endif
