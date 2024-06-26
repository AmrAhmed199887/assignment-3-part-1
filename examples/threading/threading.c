#include "threading.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

// Optional: use these functions to add debug or error prints to your application
#define DEBUG_LOG(msg,...)
//#define DEBUG_LOG(msg,...) printf("threading: " msg "\n" , ##__VA_ARGS__)
#define ERROR_LOG(msg,...) printf("threading ERROR: " msg "\n" , ##__VA_ARGS__)

void* threadfunc(void* thread_param)
{

    // TODO: wait, obtain mutex, wait, release mutex as described by thread_data structure
    // hint: use a cast like the one below to obtain thread arguments from your parameter
    //struct thread_data* thread_func_args = (struct thread_data *) thread_param;
    sleep(((struct thread_data*)thread_param)->wait_to_obtain);
    if(pthread_mutex_lock(((struct thread_data*)thread_param)->mutex_flag))
    {
       ERROR_LOG("faild to obtain mutex");
        ((struct thread_data*)thread_param)->thread_complete_success=false;
        pthread_exit(&((struct thread_data*)thread_param)->thread_complete_success);
    }
    sleep(((struct thread_data*)thread_param)->wait_to_release);
    if(pthread_mutex_unlock(((struct thread_data*)thread_param)->mutex_flag))
    {
       ERROR_LOG("faild to release mutex"); 
       ((struct thread_data*)thread_param)->thread_complete_success=false;
       pthread_exit(&((struct thread_data*)thread_param)->thread_complete_success);
    }
    ((struct thread_data*)thread_param)->thread_complete_success=true;


    return (void *)(thread_param);
}


bool start_thread_obtaining_mutex(pthread_t *thread, pthread_mutex_t *mutex,int wait_to_obtain_ms, int wait_to_release_ms)
{
    /**
     * TODO: allocate memory for thread_data, setup mutex and wait arguments, pass thread_data to created thread
     * using threadfunc() as entry point.
     * 
     *
     * return true if successful.
     *
     * See implementation details in threading.h file comment block
     */
    struct thread_data *thread_ptr=(struct thread_data*)malloc(sizeof(struct thread_data));
    thread_ptr->mutex_flag= mutex;
    thread_ptr->wait_to_obtain=(double)wait_to_obtain_ms/1000;
    thread_ptr->wait_to_release=(double)wait_to_release_ms/1000;

    int ret=pthread_create(thread,NULL,threadfunc,(struct thread_data*)thread_ptr);
    if(ret!=0)
    {
        ERROR_LOG("cany create thread");
    }
    else 
    {
        return true;
    }
    // ret=pthread_join(*thread,NULL);
    // if(ret !=0)
    // {
    //     ERROR_LOG("cant join the thread");
    // }
   


    return false;
}

