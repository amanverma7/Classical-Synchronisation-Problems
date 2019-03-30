// Aman Verma IITR CSE 17114009
// Readers_Writers Problem Solution 
// Using semaphores in C

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/errno.h>


int reader_count=0;
int item=0;
sem_t buffer_write;
sem_t mutex;

void *writer(void *arg)
{
  int g;
  g=((int)arg);
  sem_wait(&buffer_write);
  item++;
  printf("writernumber %d writes itemnumber %d \n",g,item);
  sem_post(&buffer_write);
  
}

void *reader(void *arg)
{
  int g;
  g=((int)arg);
  
  sem_wait(&mutex);
  
  reader_count=reader_count+1;
  if(reader_count==1)
    sem_wait(&buffer_write);
  
  sem_post(&mutex);
  
  printf("readernumber %d reads itemnumber %d \n",g,item);
  
  sem_wait(&mutex);
  
  reader_count=reader_count-1;
  
  if(reader_count==0)
    sem_post(&buffer_write);
  
  sem_post(&mutex);
}

int main()
{
  int i;
  
  pthread_t writer_thread_id[5];
  pthread_t reader_thread_id[5];
  sem_init(&buffer_write,0,1);
  sem_init(&mutex,0,1);
  
  for(i=0;i<=2;i++)
  {
    pthread_create(&writer_thread_id[i],NULL,writer,(void *)i);
    pthread_create(&reader_thread_id[i],NULL,reader,(void *)i);
  
  }
  
  for(i=0;i<=2;i++)
  {
    pthread_join(writer_thread_id[i],NULL);
    pthread_join(reader_thread_id[i],NULL);
    
  }

}