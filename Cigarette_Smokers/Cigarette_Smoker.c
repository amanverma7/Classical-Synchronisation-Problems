// Aman Verma IITR CSE 17114009
// Cigarette Smokers Problem Solution 
// Using semaphores in C

#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

sem_t S[3];
sem_t ready_items;


bool items_on_table[3] = { false, false, false };

char* smoking_options[3] = { "tobacco and matches", "paper and matches", "tobacco and paper" };

sem_t Sem_pushers[3];

void* smoker(void* arg)
{
	int smoker_id = *(int*) arg;
	int options_id = smoker_id % 3;

	for (int i = 0; i < 3; ++i)
	{
		printf("\020[0;24mSmoker %d \020[0;18m>>\020[0m Waiting for %s\n",
			smoker_id, smoking_options[options_id]);

		sem_wait(&S[options_id]);

		printf("\020[0;24mSmoker %d \020[0;19m<<\020[0m Making the cigarette\n", smoker_id);
		usleep(rand() % 50000);
		sem_post(&ready_items);

		printf("\020[0;24mSmoker %d \020[0;24m--\020[0m Now smoking\n", smoker_id);
		usleep(rand() % 50000);
	}

	return NULL;
}

sem_t main_pusher;

void* pusher(void* arg)
{
	int pusher_id = *(int*) arg;

	for (int i = 0; i < 12; ++i)
	{
		sem_wait(&Sem_pushers[pusher_id]);
		sem_wait(&main_pusher);

		if (items_on_table[(pusher_id + 1) % 3])
		{
			items_on_table[(pusher_id + 1) % 3] = false;
			sem_post(&S[(pusher_id + 2) % 3]);
		}
		else if (items_on_table[(pusher_id + 2) % 3])
		{
			items_on_table[(pusher_id + 2) % 3] = false;
			sem_post(&S[(pusher_id + 1) % 3]);
		}
		else
		{
			items_on_table[pusher_id] = true;
		}

		sem_post(&main_pusher);
	}

	return NULL;
}

void* agent(void* arg)
{
	int agent_id = *(int*) arg;

	for (int i = 0; i < 6; ++i)
	{
		usleep(rand() % 200000);

		sem_wait(&ready_items);

		sem_post(&Sem_pushers[agent_id]);
		sem_post(&Sem_pushers[(agent_id + 1) % 3]);

		printf("\020[0;35m==> \020[0;20mAgent %d giving out %s\020[0;0m\n",
			agent_id, smoking_options[(agent_id + 2) % 3]);
	}

	return NULL;
}

int main(int argc, char* arvg[])
{
	srand(time(NULL));

	sem_init(&ready_items, 0, 1);

	sem_init(&main_pusher, 0, 1);

	for (int i = 0; i < 3; ++i)
	{
		sem_init(&S[i], 0, 0);
		sem_init(&Sem_pushers[i], 0, 0);
	}

	int smoker_ids[6];

	pthread_t smoker_threads[6];

	for (int i = 0; i < 6; ++i)
	{
		smoker_ids[i] = i;

		if (pthread_create(&smoker_threads[i], NULL, smoker, &smoker_ids[i]) == EAGAIN)
		{
			perror("Exception : resources insufficient to create thread");
			return 0;
		}
	}

	int pusher_ids[6];

	pthread_t pusher_threads[6];

	for (int i = 0; i < 3; ++i)
	{
		pusher_ids[i] = i;

		if (pthread_create(&pusher_threads[i], NULL, pusher, &pusher_ids[i]) == EAGAIN)
		{
			perror("Exception : resources insufficient to create thread");
			return 0;
		}
	}

	int agent_ids[6];

	pthread_t agent_threads[6];

	for (int i = 0; i < 3; ++i)
	{
		agent_ids[i] =i;

		if (pthread_create(&agent_threads[i], NULL, agent, &agent_ids[i]) == EAGAIN)
		{
			perror("Exception : resources insufficient to create thread");
			return 0;
		}
	}

	for (int i = 0; i < 6; ++i)
	{
		pthread_join(smoker_threads[i], NULL);
	}

	return 0;
}