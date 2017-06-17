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

void initialize_readers_writer() {
    /*
     * Initialize the shared structures, including those used for
     * synchronization.
     */

    char buffer[INIT_BUFFER_LEN];
    strncpy(buffer, "*** INITIAL DATA ***", INIT_BUFFER_LEN);
    
    init_resource(&data, buffer);

    print_stats(&data);

    if(sem_init(&m, 0, 1) < 0)
    {
    	fprintf(stderr, "Could not initialize semaphore.\n");
    	exit(0);
    }

    readers = 0;
    writers = 0;
}


void rw_read(char *value, int len) {
    //printf("NOTHING IMPLEMENTED YET FOR rw_read\n");

    printf("Entered read.\n");

    while(writers > 0) { }
    sem_wait(&m);
    readers++;
	sem_post(&m);

    print_stats(&data);
    printf("About to read\n");

	read_resource(&data, value, len);

    printf("Finished read\n");

    print_stats(&data);
    printf("Value: (%s)\n", value);

	sem_wait(&m);
	readers--;
	sem_post(&m);

	printf("Exiting read.\n");
}


void rw_write(char *value, int len) {
    printf("NOTHING IMPLEMENTED YET FOR rw_write\n");
}
