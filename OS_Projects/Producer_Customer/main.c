#include "params.h"
#include "producer.c"
#include "customer.c"
#include "cashier.c"


int main(int argc, char *argv[])
{
	// initialization
    for (int i = 0; i < NUM_SHELF; i++) {
        pthread_mutex_init(&mutex_shelf[i], 0);
        pthread_cond_init(&cond_shelf[i], NULL);
    }

    for (int i = 0; i < NUM_CASHIER; i++) {
        pthread_mutex_init(&mutex_counter[i], 0);
        pthread_mutex_init(&mutex_cashier[i], 0);
    }

    // fill all shelf
    for (int i = 0; i < NUM_SHELF; i++)
        cap_shelf[i] = MAX_CAPACITY;

    // empty all counter
    for (int i = 0; i < NUM_CASHIER; i++) {
        queue_cashier[i] = 0;
        if_cashier[i] = 1;
    }

    pthread_t p_producer;
	pthread_t p_customer[NUM_CUSTOMER];
    pthread_t p_cashier[NUM_CASHIER];
    
	int customer_id[NUM_CUSTOMER];
    int cashier_id[NUM_CASHIER];

    int rc;
	
	// create all threads
	rc = pthread_create(&p_producer, NULL, producer, NULL);
	if (rc) {
        printf("Error: return code from pthread_create(p_producer) is %d.\n", rc);
        exit(-1);
    }

    for (int i = 0; i < NUM_CASHIER; i++) {
        cashier_id[i] = i;
		rc = pthread_create (&p_cashier[i], NULL, cashier, &cashier_id[i]);
		if (rc) {
            printf("Error: return code from pthread_create(p_cashier) is %d.\n", rc);
            exit(-1);
        }
    }

    for (int i = 0; i < NUM_CUSTOMER; i++) {
        customer_id[i] = i + 1;
        rc = pthread_create(&p_customer[i], NULL, customer, &customer_id[i]);
        if (rc) {
            printf("Error: return code from pthread_create(p_customer) is %d.\n", rc);
            exit(-1);
        }
        sleep(rand() % TIME_NEW_CUSTOMER);
    }

	void *status;
	// wait customer threads end
    for (int i = 0; i < NUM_CUSTOMER; i++) {
        rc = pthread_join(p_customer[i], &status);
        if (rc) {
            printf("Error: return code from pthread_join(p_customer) is %d.\n", rc);
            exit(-1);
        }
	} 

	sleep(TIME_IDLE);

    // destory mutex and condition
    for (int i = 0; i < NUM_SHELF; i++) {
        pthread_mutex_destroy(&mutex_shelf[i]);
        pthread_cond_destroy(&cond_shelf[i]);
    }

    for (int i = 0; i < NUM_CASHIER; i++) {
        pthread_mutex_destroy(&mutex_counter[i]);
        pthread_mutex_destroy(&mutex_cashier[i]);
        pthread_cond_destroy(&cond_cashier[i]);
    }

	return 0;
}
