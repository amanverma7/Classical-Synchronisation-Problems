// Aman Verma IITR CSE 17114009
// Producer Consumer Problem Solution 
// Using semaphores in C

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#define BUFFER_SIZE 4
#define FULL 0
#define EMPTY 0

sem_t empty; 
sem_t full; 

char buffer[BUFFER_SIZE];
int IN = 0;
int OUT = 0;

void produce(char item)
{
  int value;
  sem_wait(&empty); 

  buffer[IN] = item;
  IN = (IN + 1) % BUFFER_SIZE;
  printf("\t...now_produces %c ...IN %d..alphabet_produced=%d\n",item,IN,item);
  if(IN==FULL)
    {
      sem_post(&full);
      sleep(1);
    }
  sem_post(&empty);

}

void * Producer()
{
  int i;
  for(i = 0; i < 10; i++)
    {
      produce((char)('A'+ i % 26));
    }
}

void consume()
{
  int item;

  sem_wait(&full); 

  item = buffer[OUT];
  OUT = (OUT + 1) % BUFFER_SIZE;
  printf("\t...now_consumes %c ...OUT %d..alphabet_consumed=%d\n",item,OUT,item);
  if(OUT==EMPTY) 
    {
      sleep(1);
    }

  sem_post(&full);
}

void * Consumer()
{
  int i;
  for(i = 0; i < 10; i++)
    {
      consume();
    }
}

int main()
{
  pthread_t producer_thread_id,consumer_thread_id;

  sem_init(&empty,0,1);
  sem_init(&full,0,0);


  if(pthread_create(&producer_thread_id, NULL,Producer, NULL))
    {
      printf("\n Exception: error while creating thread 1");
      exit(1);
    }

  if(pthread_create(&consumer_thread_id, NULL,Consumer, NULL))
    {
      printf("\n Exception: error while creating thread 2");
      exit(1);
    }

  if(pthread_join(producer_thread_id, NULL)) 
    {
      printf("\n Exception: error while joining thread");
      exit(1);
    }

  if(pthread_join(consumer_thread_id, NULL)) 
    {
      printf("\n Exception: error while joining thread");
      exit(1);
    }

  sem_destroy(&empty);
  sem_destroy(&full);

  pthread_exit(NULL);
  return 1;
}