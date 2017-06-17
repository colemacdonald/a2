/*Required Headers*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include "rw.h"
#include "resource.h"

/*
 * Declarations for reader-writer shared variables -- plus concurrency-control
 * variables -- must START here.
 */

static resource_t 	data;
int 				readers;
int 				writers;
//int 				active_writers;
sem_t 				m;
sem_t 				write;

void initialize_readers_writer() {
    /*
     * Initialize the shared structures, including those used for
     * synchronization.
     */

    char buffer[30];
    strncpy(buffer, "*** INITIAL DATA ***\0", 21);
    
    init_resource(&data, buffer);

    print_stats(&data);

    if(sem_init(&m, 0, 1) < 0)
    {
    	fprintf(stderr, "Could not initialize semaphore.\n");
    	exit(0);
    }

    readers = 0;
    writers = 0;
    // //active_writers = 0;

    // // initialize reader semaphore for threads (0) [vs processes (>0)], with init value 1
    // // if(sem_init(&read, 0, 1) < 0)
    // // {
    // // 	fprintf(stderr, "Could not initialize 'read' semaphore.\n");
    // // 	exit(0);
    // // }

    // // // initialize reader semaphore for threads (0) [vs processes (>0)], with init value 1
    // if(sem_init(&write, 0, 1) < 0)
    // {
    // 	fprintf(stderr, "Could not initialize 'write' semaphore.\n");
    // 	exit(0);
    // }
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
 //    printf("Entered write.\n");

	// sem_wait(&write);

	// while(readers > 0) { }

	// writers++;
	// write_resource(&data, value, len);
	// writers--;

	// sem_post(&write);

	// printf("Exiting write.\n");
}
