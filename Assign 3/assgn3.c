#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "log.h"

int logindex = 0;
int *logi = &logindex;
// mutex for logging functions required for thread-safe code
pthread_mutex_t tlock = PTHREAD_MUTEX_INITIALIZER;

int no_of_threads = 0;
#define ARRAYSIZE  1000000
int no_of_items = 0;



//TO DO:  Here go your global variables
//TO DO:  declare and initialize global mutex
unsigned long arr[ARRAYSIZE]; 
unsigned long global_sum;
pthread_mutex_t sum_lock = PTHREAD_MUTEX_INITIALIZER;

void init_arr() {
  srand(time(NULL));
  long i;
  for(i = 0; i < ARRAYSIZE; i++) {
    arr[i] = (long) (rand() % 10);
  }
}


// function doit ----------------------------------------
void* doit(void *params) {
  // TO DO: here goes your local variables
  // TO DO: get from param your thread index
  long from;
  long to;
  long thread_index = *((long *) params);
  long local_sum = 0;
  long i;

  // TO DO: find your slice of the array -- from and to

  from = thread_index * no_of_items;
  to = from + no_of_items - 1;

  if (to > ARRAYSIZE){
    to = ARRAYSIZE;
  }
 
  // TO DO: display the message about which part of the array this threads does
  // TO DO: log the message about which part of the array this threads does

  msg("Thread %u processing the array from %u to %u", thread_index, from, to);
  Msg("Thread %u processing the array from %u to %u", thread_index, from, to);
 
  // TO DO: sum up your portion of the array arr[]

  for(i = from; i <= to; i++) {
    local_sum += arr[i];
  }

  // TO DO: display the message about your summation
  // TO DO: log the message about your summation
 
  msg("Thread %u summation is %u", thread_index, local_sum);
  Msg("Thread %u summation is %u", thread_index, local_sum);

  // TO DO: lock your global mutex
  // TO DO: update the global sum
  // TO DO: unlock your global mutex
  // TO DO: exit the thread

  pthread_mutex_lock(&sum_lock);
  global_sum += local_sum;
  pthread_mutex_unlock(&sum_lock);

  pthread_exit(NULL);
  

}//end doit


// function main ----------------------------------------
int main(int argc,char *argv[]) {
  // TO DO: here go your locat variables
  
  pthread_t* tid;
  long* thread_index;
  long sum = 0;
  long i;


  //TO DO:  check args, argv if error, display message and terminate the program
  //TO DO:  set the number of threats to be dispatched
  //TO DO:  check it is between 2 and 10 inclusive, if not error and terminate

  if (argc != 2){
    msg("wrong number of command line arguments");
    msg_exit("usage - %s <number of threads>",argv[0]);
  }

  no_of_threads = atoi(argv[1]);

  if ( no_of_threads < 2 || no_of_threads > 10 ) {
    msg("wrong number of threads, must be 2 - 10");
    msg_exit("usage - %s <number of threads>",argv[0]);
  }

  create_log("assgn3.log");

  init_arr();

  //TO DO: determine no_of_items each thread is supposed to sum up
  //TO DO: create dynamically array to hold the index of each dispatched thread
  //TO DO: create dynamically array to hold thread id of each dispatched thread

  if (ARRAYSIZE % no_of_threads == 0) {
    no_of_items = ARRAYSIZE/no_of_threads;
    } 

  else { 
    no_of_items = 1 + (ARRAYSIZE/no_of_threads);
  } 

  thread_index = (long*)malloc(no_of_items * sizeof(long));
  tid = (pthread_t*)malloc(no_of_items * sizeof(pthread_t));

  //TO DO: in a loop, dispatch each thread to execute doit() (see pthread_create())
  //TO DO: and display and log message "dispatched thread XXX with tid = YYY"

  for(i = 0; i < no_of_threads; i++) {
    thread_index[i] = i;
    if ((pthread_create(&tid[i], NULL, doit, &thread_index[i])) != 0) {
        msg_exit("%u couldnt create a thread", &tid[i]);
    }
    msg("dispatched thread %u with tid = %u", i, tid[i]);
    Msg("dispatched thread %u with tid = %u", i, tid[i]);
  }


  //TO DO:  wait for all threads to complete
  //TO DO:  display message "Overall sum = XXX"

  for(i = 0; i < no_of_threads; i++){ 
    if (pthread_join(tid[i], NULL) != 0) {
        msg_exit("Oh dear, something went wrong with pthread_join %s\n", strerror(errno));
    }
  } 

  msg("Overall sum = %u", global_sum);

  //TO DO: sum up the array arr[]
  //TO DO: and display the message "Check sum = XXX"
  for(i = 0; i < ARRAYSIZE; i++){
      sum += arr[i]; 
  }

  msg("Check sum = %u", sum);

   free(thread_index);
   free(tid);



  return 0;
}//end main
