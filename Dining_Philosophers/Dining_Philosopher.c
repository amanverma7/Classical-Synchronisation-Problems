// Aman Verma IITR CSE 17114009
// Dining Philosopher Problem Solution 
// Using semaphores in C
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

 
#define N 5
//three states of a philosopher
#define THINKING 2
#define HUNGRY 1
#define EATING 0
//
#define LEFT (p_index + 4)%N
#define RIGHT (p_index + 1)%N
 
int p_array[N] = {0, 1, 2, 3, 4}; 
int state[N];

sem_t mutex;
sem_t sem_array[N];
 
void util(int p_index)
{
    if (state[p_index] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING)
    {
        state[p_index] = EATING;
        sleep(2);
        printf("philospher_number %d pickups fork number %d and %d\n", p_index + 1, LEFT + 1, p_index + 1);
        printf("philospher_number %d is eating\n", p_index + 1);

        sem_post(&sem_array[p_index]);
    }
}


void pickup(int p_index)
{
    sem_wait(&mutex);
    
    state[p_index] = HUNGRY;
    printf("philospher_number %d is Hungry\n", p_index + 1);

   
    util(p_index);
    
    sem_post(&mutex);

    
    sem_wait(&sem_array[p_index]);
    sleep(1);
}
 

void put_down(int p_index)
{
    sem_wait(&mutex); 
    
    state[p_index] = THINKING;
    printf("philospher_number %d putting", p_index + 1);
  printf(" fork_number %d and %d down\n", LEFT + 1, p_index + 1);
    printf("philospher_number %d is thinking\n", p_index + 1);
    util(LEFT);
    util(RIGHT);
    
    sem_post(&mutex);
}

void *create_philospher(void *num)
{
    while(1)
    {
        int *i = num;
        sleep(1);
        pickup(*i);
        sleep(0);
        put_down(*i);
    }
}
 
int main()
{
    pthread_t thread_index[N];

    int i;

    sem_init(&mutex, 0, 1);
    for(i = 0; i < N; i++)
        sem_init(&sem_array[i], 0, 0);
    for(i = 0; i < N; i++)
    {
        pthread_create(&thread_index[i], NULL, create_philospher, &p_array[i]);
        printf("philosopher_number %d is thinking\n", i + 1);
    }
    for(i = 0; i < N; i++)
        pthread_join(thread_index[i], NULL);
    return 0;
}