/* Author:      Matt W. Martin, 4374851
 *              kaethis@tasmantis.net
 *
 * Project:     CS3790, Lab 03:
 *              Producer-Consumer Solution
 *		C++ Implementation
 *
 * File:        prog.cpp */

#include <mutex>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>
#include <vector>


const int  BUFFER_SIZE = 10;		// Size of hypothetical bounded-buffer.

int  items = 0;				/* Represents the number of (non-existant)
					 *  items in the buffer.  This value should
					 *  NEVER exceed the size of the buffer, and
					 *  should NEVER, EVER drop below 0. */

sem_t  avail;				/* Signifies the number of items in the
					 *  buffer available to consume. */

sem_t  empty;				/* Signifies the number of available spaces
					 *  in the buffer for an item to be produced
					 *  and placed. */

std::mutex  mut;			/* Implements mutual-exclusion inside
					 *  CRITICAL REGION to avoid hypothetical
					 *  RACE CONDITION (see README.txt). */


void work(){
/* This FUNCTION simulates some sort of undetermined number of unknown instructions
 *  executed by the THREAD.  In reality, all it does is put the THREAD to sleep
 *  for an indescriminate amount of time (between 1 and 5 seconds). */

	int  time = (rand() % 5 + 1);

	sleep(time);
}

void produce(){

	work();

	items++;
}

void consume(){

	work();

	items--;
}

void *producer(void *param){

	int  tid = (int)param;


  begin_producer:

	work();


	sem_wait(&empty);

	  fprintf(stderr, "\nPROD#%d is entering CRITICAL REGION!\n", tid);

	  mut.lock();

	    fprintf(stderr, "\n PROD#%d is producing an item...\n", tid);

	    produce();

	    fprintf(stderr, "\n Items in buffer is now %d.\n", items);

	  mut.unlock();

	  fprintf(stderr, "\nPROD#%d is exiting CRITICAL REGION!\n", tid);

	sem_post(&avail);


	work();

	goto begin_producer;
}

void *consumer(void *param){

	int tid = (int)param;


  begin_consumer:

	work();


	sem_wait(&avail);

	  fprintf(stderr, "\nCONS#%d is entering CRITICAL REGION!\n", tid);

	  mut.lock();

	    fprintf(stderr, "\n CONS#%d is consuming an item...\n", tid);

	    consume();

	    fprintf(stderr, "\n Items in buffer is now %d.\n", items);

	  mut.unlock();

	  fprintf(stderr, "\nCONS#%d is exiting CRITICAL REGION!\n", tid);

	sem_post(&empty);


	work();

	goto begin_consumer;
}

int main(int argc, char* argv[]){


	if(argc != 3){

		fprintf(stderr, "\nINVALID NUMBER OF ARGS!  EXITING.\n");
		fprintf(stderr, "NOTE:  %s <prod_num> <cons_num>\n\n", argv[0]);

		exit(-1);
	}


	int  prod_num = atoi(argv[1]);

	int  cons_num = atoi(argv[2]);


	pthread_t  p_tid[prod_num],
	           c_tid[cons_num];


	sem_init(&avail, NULL, 0);

	sem_init(&empty, NULL, BUFFER_SIZE);


	srand(time(NULL));


	for(int i = 0; i < prod_num; i++)
		pthread_create(&p_tid[i], NULL, producer, (void*)i);

	for(int i = 0; i < cons_num; i++)
		pthread_create(&c_tid[i], NULL, consumer, (void*)i);


  begin_main:

	/* NOTE: In order to avoid the MAIN THREAD from falling-out and terminating
	 *	  the process while the producer & consumer THREADS are performing
	 *	  their FUNCTIONS, this THREAD undergoes an infinite-loop (the above
	 *	  FUNCTIONS also implement infinite-loops, hence the omission of a
	 *	  JOIN).  As a result, the USER is expected to kill this process
	 *	  manually in order to exit the program. */

	goto begin_main;


	return 0;
}
