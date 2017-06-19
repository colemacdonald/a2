/*Required Headers*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include "meetup.h"
#include "resource.h"

/*
 * Declarations for barrier shared variables -- plus concurrency-control
 * variables -- must START here.
 */

int             group_size;
int             meet_order;
pthread_mutex_t m;
pthread_cond_t  barrier_q;
resource_t      code;

int             count;
int             gen;
int             num_read;
int             reading_gen;
int             written;


/*
 * Initialize all variables above.
 * Set count and gen to 0
 */
void initialize_meetup(int n, int mf) {
    char label[100];
    int i;

    if (n < 1) {
        fprintf(stderr, "Who are you kidding?\n");
        fprintf(stderr, "A meetup size of %d??\n", n);
        exit(1);
    }

    /*
     * Initialize the shared structures, including those used for
     * synchronization.
     */

    group_size = n;
    meet_order = mf;

    count = 0;
    gen = 0;
    num_read = 0;
    reading_gen = 0;
    written = 0;

    init_resource(&code, "code");

    if(pthread_mutex_init(&m, NULL) != 0)
    {
       fprintf(stderr, "Could not initialize mutex 'm'\n");
       exit(0);
    }
    if(pthread_cond_init(&barrier_q, NULL) != 0)
    {
       fprintf(stderr, "Could not initialize condition variable 'barrier_q'\n");
    }
}


void join_meetup(char *value, int len) {
    //printf("NOTHING IMPLEMENTED YET FOR join_meetup\n");
    pthread_mutex_lock(&m);
    count++;

    int me = count;
    int my_gen = gen;

    printf("%s entered with gen = %d and count = %d\n", value, gen, count);

    if( (me == 1 && meet_order == MEET_FIRST) || (me == group_size && meet_order == MEET_LAST) )
    {

        printf("%s wants to write\n", value);
        while(num_read < group_size && gen != 0)
            pthread_cond_wait(&barrier_q, &m);

        num_read = 0;
        reading_gen = gen;
        printf("%s Writing - Gen: %d Count: %d Reading_gen: %d Num_read: %d\n", value, gen, me, reading_gen, num_read);
        write_resource(&code, value, len);
        written = 1;
        pthread_cond_broadcast(&barrier_q);
    }

    if(count < group_size)
    {
        my_gen = gen;
        while(my_gen == gen || my_gen != reading_gen)
            pthread_cond_wait(&barrier_q, &m);

        printf("%s Reading - Gen: %d Count: %d Reading_gen: %d Num_read: %d\n", value, my_gen, me, reading_gen, num_read);
        read_resource(&code, value, len);
        num_read++;
        if(num_read == group_size)
            pthread_cond_broadcast(&barrier_q);
    }
    else
    {
        while(written == 0)
            pthread_cond_wait(&barrier_q, &m);

        printf("%s Reading and Broadcast - Gen: %d Count: %d Reading_gen: %d Num_read: %d\n", value, gen, me, reading_gen, num_read);
        count = 0;
        gen++;
        written = 0;
        read_resource(&code, value, len);
        num_read++;
        pthread_cond_broadcast(&barrier_q);
    }
    pthread_mutex_unlock(&m);
}
