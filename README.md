# CSCI 4061 Project 3 - Multithreading

## Project Group Number: 27  

Reed Fazenbaker (fazen007)  
Mikkel Folting (folti002)  
Note: the extra credit portion of this project was attempted.

## Contributions

Mikkel Folting: Worked on producer-consumer synchronization, shared queue implementation, and consumer reading of packets  
Reed Fazenbaker: Worked on main.c, initializing mutual exclusion variables, initializing threads, and parsing data

## Assumptions

No significant assumptions were made outside the project specification.  

## Compiling the program

```bash
make
```

Type the above command into the home directory of the project.

## Running the program

```bash
./bank [number of consumers] [input file]
```

OR  

```bash
./bank [number of consmers] [inputFile] [option] [queue size]
```

The above commands are both valid to run the program.  
The number of consumers to create and the input file to read from must be specified.  
The program can also be run in 3 additional modes, specified with the option input.  
-b indicates that the program should be run using a bounded buffer, the size of which is specified in the next argument: queue size.  
-p indicates that the program should log its progress to a file called log.txt in the output folder of the project.  
-bp indicates that the program should run using a bounded buffer and the progress should be logged to log.txt.  

## Test Machine Information

test machine: csel-kh1250-01.cselabs.umn.edu  
group number: G[27]  
names: Reed Fazenbaker, Mikkel Folting  
x500: fazen007, folti002
