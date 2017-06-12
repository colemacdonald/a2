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
sem_t 				m;
int 				readers;
int 				writers;
// sem_t read;
// sem_t write;

void initialize_readers_writer() {
    /*
     * Initialize the shared structures, including those used for
     * synchronization.
     */
    init_resource(&data, "");

    if(sem_init(&m, 0, 1) < 0)
    {
    	fprintf(stderr, "Could not initialize semaphore.\n");
    	exit(0);
    }

    readers = 0;
    writers = 0;

    // initialize reader semaphore for threads (0) [vs processes (>0)], with init value 1
    // if(sem_init(&read, 0, 1) < 0)
    // {
    // 	fprintf(stderr, "Could not initialize 'read' semaphore.\n");
    // 	exit(0);
    // }

    // // initialize reader semaphore for threads (0) [vs processes (>0)], with init value 1
    // if(sem_init(&write, 0, 1) < 0)
    // {
    // 	fprintf(stderr, "Could not initialize 'write' semaphore.\n");
    // 	exit(0);
    // }
}


void rw_read(char *value, int len) {
    printf("NOTHING IMPLEMENTED YET FOR rw_read\n");

    sem_wait(&m);

}


void rw_write(char *value, int len) {
    printf("NOTHING IMPLEMENTED YET FOR rw_write\n");
}
