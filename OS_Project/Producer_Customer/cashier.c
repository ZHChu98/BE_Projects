#include "params.h"

void *cashier(void *argv)
{
	int id = *(int*)argv;
	while(1) {
        if (queue_cashier[id] == 0) {
            pthread_mutex_lock(mutex_counter + id);
            printf("Cashier %d leaves the counter since no one waits there.\n", id + 1);
            pthread_mutex_lock(mutex_cashier + id);
            if_cashier[id] = 0;
            pthread_mutex_unlock(mutex_cashier + id);
            gettimeofday(&now, NULL);
            time2wait.tv_sec = now.tv_sec + TIME_IDLE;
            pthread_cond_timedwait(cond_cashier + id, mutex_counter + id, &time2wait);
            printf("Cashier %d goes back to work.\n", id + 1);
            pthread_mutex_unlock(mutex_counter + id);
        }
        sleep(rand() % TIME_IDLE);
    }
}
