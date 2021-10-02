#ifndef PARAMS_H
#define PARAMS_H

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <semaphore.h>

#define NUM_CUSTOMER 30
#define NUM_SHELF 5
#define NUM_CASHIER 2

#define MAX_CAPACITY 5

#define TIME_NEW_CUSTOMER 3
#define TIME_WORK 5
#define TIME_IDLE 7
#define TIME_FIND_PACKAGE 4
#define TIME_FIND_CASHIER 3
#define TIME_CHECK_OUT 4

struct timespec time2wait;
struct timeval now;

// to represent shelf
pthread_mutex_t mutex_shelf[NUM_SHELF];

// to represent counter
pthread_mutex_t mutex_counter[NUM_CASHIER];

// to record if cashier leaves the counter
pthread_mutex_t mutex_cashier[NUM_CASHIER];

// signal variable
pthread_cond_t cond_shelf[NUM_SHELF];
pthread_cond_t cond_cashier[NUM_CASHIER];

// to count how many packages does the shelf have 
int cap_shelf[NUM_SHELF];

// to count how many people is waiting in the queue to check out
int queue_cashier[NUM_CASHIER];
int if_cashier[NUM_CASHIER];

#endif /* PARAMS_H */
