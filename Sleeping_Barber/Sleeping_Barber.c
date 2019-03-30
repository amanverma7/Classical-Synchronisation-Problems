// Aman Verma IITR CSE 17114009
// Sleeping Barber Problem Solution 
// Using semaphores in C

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/errno.h>

#define _REENTRANT
#define MAX_CUSTOMERS 20

void *customer(void *num);
void *barber(void *junk) ;
void wait_random(int secs);

sem_t chair;
sem_t pillow;
sem_t waitingRoom;   
sem_t seat_wait;

int finished = 0;

int main(int argc, char *argv[]) {

    pthread_t barber_thread_id;
    pthread_t thread_id[MAX_CUSTOMERS];
    int Number[MAX_CUSTOMERS];
    long RandSeed;
    int i, customer_num, chair_num;
    
    
    if (argc != 4) {
  printf("Prototype is wrong\n");
  exit(-1);
    }
    
    // Command Line Arguments
    customer_num = atoi(argv[1]);
    chair_num = atoi(argv[2]);
    RandSeed = atol(argv[3]);
    
    if (customer_num > MAX_CUSTOMERS) {
  printf("The maximum customers possible are %d.\n", MAX_CUSTOMERS);
  exit(-1);
    }
    
    printf("SOLUTION.\n");
    
    srand48(RandSeed);

    for (i=0; i<MAX_CUSTOMERS; i++) {
  Number[i] = i;
    }
    
    sem_init(&waitingRoom, 0, chair_num);
    sem_init(&chair, 0, 1);
    sem_init(&pillow, 0, 0);
    sem_init(&seat_wait, 0, 0);
    
    pthread_create(&barber_thread_id, NULL, barber, NULL);  //creating the barber

    
    for (i=0; i<customer_num; i++) {
  pthread_create(&thread_id[i], NULL, customer, (void *)&Number[i]);  //creating the customer
    }

    for (i=0; i<customer_num; i++) {
  pthread_join(thread_id[i],NULL);
    }

    finished = 1;
    sem_post(&pillow);  
    pthread_join(barber_thread_id,NULL);  
}

void *barber(void *junk) {
    while (!finished) {

    printf("The barber is sleeping\n");
    sem_wait(&pillow);

    if (!finished) {

        printf("Barber is cutting hair\n");
        wait_random(3);
        printf("Barber has finished up .\n");

        sem_post(&seat_wait);
    }
    else {
        printf("Barber is leaving.\n");
    }
    }
}


void *customer(void *number) {
    int num = *(int *)number;

    printf("customer_number %d leaving for barber shop.\n", num);
    wait_random(5);
    printf("customer_number %d arrived at barber shop.\n", num);

    if (sem_trywait(&waitingRoom) == -1) {
        printf("customer_number %d is leaving coz the waiting room is full.\n",num);
        return 0;
    }

    printf("customer_number %d enters the waiting room.\n", num);

    sem_wait(&chair);
    
    sem_post(&waitingRoom);

    printf("customer_number %d waking up the barber.\n", num);
    sem_post(&pillow);

    sem_wait(&seat_wait);
    sem_post(&chair);
    printf("customer_number %d leaving the barber shop.\n", num);
}

void wait_random(int secs) {
    int len;
    len = (int) ((drand48() * secs) + 1);
    sleep(len);
}