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

// help to ensure proper threads get write access to the code
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

/*
 *  Function called to join the meetup with code word value.
 *  Threads start by gathering information on "who" they are (ie. the corresponding count and gen)
 *  In meet first, each thread with me (count) == 1 gets write privilege and all others only read,
 *  the me and my_gen variables help ensure that the first thread to call the meetup function gets to write regardless
 *  of order of context switching
 *
 *  meetlast operates very similarly except only thread with me (count) == group_size
 *
 *  If a thread is attempting to read but its generation's code has not yet been written it will wait, this ensures
 *  that a thread does not read the codeword of the previous generation (occured with certain context switches)
 *
 *  If a thread wants to write (it is count 0 of its generation) but not every thread from previous generation has read yet,
 *  it will wait to write until each thread has read
 */
void join_meetup(char *value, int len) {
    pthread_mutex_lock(&m);

    count++;

    int me = count;
    int my_gen = gen;

    printf("%s entered with count: %d, gen: %d\n", value, me, my_gen);

    if( (me == 1 && meet_order == MEET_FIRST) || (me == group_size && meet_order == MEET_LAST) )
    {
        while(num_read < group_size && gen != 0)
            pthread_cond_wait(&barrier_q, &m);

        num_read = 0;
        reading_gen = gen;

        write_resource(&code, value, len);
        written = 1;

        //ensure threads from this gen that are waiting on write get woken
        pthread_cond_broadcast(&barrier_q);
    }

    if(me < group_size)
    {
        my_gen = gen;
        while(my_gen == gen || my_gen != reading_gen)
            pthread_cond_wait(&barrier_q, &m);

        read_resource(&code, value, len);
        num_read++;

        //last one supposed to read
        if(num_read == group_size)
            pthread_cond_broadcast(&barrier_q);
    }
    else
    {
        while(written == 0)
            pthread_cond_wait(&barrier_q, &m);

        count = 0;
        gen++;
        written = 0;
        read_resource(&code, value, len);
        num_read++;
        pthread_cond_broadcast(&barrier_q);
    }
    pthread_mutex_unlock(&m);
}
