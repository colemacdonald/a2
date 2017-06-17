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
}


void rw_read(char *value, int len) {
    printf("NOTHING IMPLEMENTED YET FOR rw_read\n");
}


void rw_write(char *value, int len) {
    printf("NOTHING IMPLEMENTED YET FOR rw_write\n");
}
