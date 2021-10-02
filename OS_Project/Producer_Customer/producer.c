#include "params.h"

void *producer()
{
	while(1)
	{
        // check which shelf needs maintenance
        for (int i = 0; i < NUM_SHELF; i++) {
            if (cap_shelf[i] == 0) {
                printf("The producer is maintaining shelf <%d>.\n", i + 1);
                pthread_mutex_lock(mutex_shelf + i);
                cap_shelf[i] = MAX_CAPACITY;
                printf("The producer has filled up shelf <%d> and decides to relax a while.\n", i + 1);
                pthread_mutex_unlock(mutex_shelf + i);
                pthread_cond_broadcast(cond_shelf + i);
            }
        }

        // let the cashier back to work if needed
        for (int i = 0; i < NUM_CASHIER; i++) {
            if (queue_cashier[i] != 0 && if_cashier[i] == 0) {
                pthread_cond_broadcast(cond_cashier + i);
                printf("The producer has called cashier %d back to work.\n", i + 1);
                pthread_mutex_lock(mutex_cashier + i);
                if_cashier[i] = 1;
                pthread_mutex_unlock(mutex_cashier + i);
            }
        }
	}
}
