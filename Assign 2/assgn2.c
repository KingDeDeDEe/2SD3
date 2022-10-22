/**
 * Assignment 2
 * @author Muzmail Janjua
 * @course CompSci 2SD3
 * @date March 13th 2022 
 */

/* Standard header files that need to be included */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "log.h"

#define MAX_PERSONS 8
#define MIN_PERSONS 1
#define INPUT_SIZE  100

// STEP 1: Define global variables for logging functions:
int logindex = 0;
int * logi = &logindex;
pthread_mutex_t tlock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t plock = PTHREAD_MUTEX_INITIALIZER;

void* conversation(void*);

typedef struct {
    int index;
    pthread_t tid;
    char name[10];
    int linecount;
    int done;
} PERSON;
PERSON persons[MAX_PERSONS];
int person_counter;

/**
 * Main function
 */
int main(int argc, char const *argv[]) {

    int i;
    if (_level < MIN_PERSONS || _level > MAX_PERSONS) {
        msg("The program was compiled with a wrong level permitted levels are 1 .. 8\n");
        return 1;
    } else if (_level == 1) {
        msg("There is only 1 personality: Person 0");
    } else {
        for (i = 0; i < _level; i++)
        {
            msg("Person_%d", i);
        }
        msg("");
    }

    create_log("assgn2.log");

    for (i = 0; i < _level; i++) {
        persons[i].index = i;
        persons[i].tid = 0; 
        sprintf(persons[i].name,"Person%d",i); // char name[10];
        persons[i].linecount = 0;
        persons[i].done = -1;
    }
    
    person_counter = 0;

    do {
        for (i = 0; i < _level; i++) {
            if (persons[i].done == 1) {
                continue;
            }
            if ((pthread_create(&persons[i].tid, NULL, conversation, &persons[i])) != 0) {
                msg_exit("%s couldnt create a thread", persons[i].name);
            }
        }
        for (i = 0; i < _level; i++) {
            if (persons[i].done == 1) {
                continue;
            }
            if (pthread_join(persons[i].tid, NULL) != 0) {
                msg_exit("Oh dear, something went wrong with pthread_join %s\n", strerror(errno));
            }
        }
    } while (person_counter != 0);

    for (i = 0; i < _level; i++) {
        msg("%s processed %d lines of messages", persons[i].name, persons[i].linecount);
    }
    return 0;
}

void * conversation (void * param) {
    void* ret = NULL;
    char user_input[INPUT_SIZE];
    PERSON * persons;
    persons = (PERSON*) param;

    pthread_mutex_lock(&plock);
    if (persons->done == -1){
        persons->done = 0;
        person_counter++;
    }
    msg("[pid=%u,tid=%u,index=%u,name=%s] Enter Message:", getpid(), pthread_self(), persons->index, persons->name);
    fgets(user_input, INPUT_SIZE, stdin);
    user_input[strlen(user_input) - 1] = '\0';
    pthread_mutex_unlock(&plock);

    // If the response started with 'quit', then:
    if (strncmp(user_input, "quit", 4) == 0) {
        pthread_mutex_lock(&plock);
        persons->done = 1;
        person_counter--;
        pthread_mutex_unlock(&plock);
        Msg("[pid=%u,tid=%u,index=%u,name=%s] Enter Message:\nUser quit", getpid(), pthread_self(), persons->index, persons->name);
        pthread_exit(ret);
    }
    else {
        persons->linecount++;
        Msg("[pid=%u,tid=%u,index=%u,name=%s] Enter Message:\n%s", getpid(), pthread_self(), persons->index, persons->name, user_input);
        persons->done = 0;
        pthread_exit(ret);
    }
}
