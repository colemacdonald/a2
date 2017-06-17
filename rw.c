/*Required Headers*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include "rw.h"
#include "resource.h"


#define INIT_BUFFER_LEN 30

/*
 * Declarations for reader-writer shared variables -- plus concurrency-control
 * variables -- must START here.
 */

static resource_t data;
int readers;
int writers;

sem_t m;
sem_t w;

void initialize_readers_writer() {
    /*
     * Initialize the shared structures, including those used for
     * synchronization.
     */

    char buffer[INIT_BUFFER_LEN];
    strncpy(buffer, "rw_data", INIT_BUFFER_LEN);
    
    init_resource(&data, buffer);

    if(sem_init(&m, 0, 1) < 0)
    {
    	fprintf(stderr, "Could not initialize semaphore 'm'\n");
    	exit(0);
    }

    if(sem_init(&w, 0, 1) < 0)
    {
    	fprintf(stderr, "Could not initialize semaphore 'w'\n");
    	exit(0);
    }

    readers = 0;
    writers = 0;
}


void rw_read(char *value, int len) {
    while(writers > 0) { }
    sem_wait(&m);
    readers++;
	sem_post(&m);

	read_resource(&data, value, len);

	sem_wait(&m);
	readers--;
	sem_post(&m);
}


void rw_write(char *value, int len) {
	sem_wait(&w);

	writers++;

	while(readers > 0) {}

    write_resource(&data, value, len);

	writers--;

	sem_post(&w);
}
