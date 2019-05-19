#include "params.h"

void *customer(void *argv) 
{
	// customer id
	int id = *(int*)argv;
	
	// target shelf id
	int target = rand() % NUM_SHELF;

    printf("Customer %d needs to go to shelf <%d>.\n", id, target + 1);

    // find the package at shelf
    sleep(TIME_FIND_PACKAGE);
    pthread_mutex_lock(mutex_shelf + target);
    if (cap_shelf[target] == 0)
        pthread_cond_wait(cond_shelf + target, mutex_shelf + target); 
    cap_shelf[target] -= 1;
    printf("Customer %d has found the package.\n", id);
    pthread_mutex_unlock(mutex_shelf + target);

    // find cashier with least customer
    pthread_mutex_lock(mutex_counter);
    pthread_mutex_lock(mutex_counter + 1);
    int queue = queue_cashier[0] <= queue_cashier[1] ? 0 : 1;
    queue_cashier[queue] += 1;
    printf("Customer %d is checking out at cashier %d.\n", id, queue + 1);
    pthread_mutex_unlock(mutex_counter);
    pthread_mutex_unlock(mutex_counter + 1);
    sleep(TIME_FIND_CASHIER);

    // check out at cashier counter
    pthread_mutex_lock(mutex_cashier + queue);
    queue_cashier[queue] -= 1;
    sleep(TIME_CHECK_OUT);
    pthread_mutex_unlock(mutex_cashier + queue);
    

    // customer leaves and thread exit
	printf("Customer %d leaves.\n", id);

	pthread_exit(NULL);
}