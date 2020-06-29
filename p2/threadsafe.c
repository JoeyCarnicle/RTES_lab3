#include <pthread.h>
#include <stdio.h>
#include <sched.h>
#include <time.h>
#include <stdlib.h>

struct timespec start_time;

pthread_t thread1, thread2;

pthread_mutex_t mutex;

//void * update(void);
//void * read(void);

struct state
{
	int x;
	int y;
	int z;
	struct timespec time;
}state;

void *update(void *thread)
{
	pthread_mutex_lock(&mutex);
	state.x++;
	state.y++;
	state.z++;
	clock_gettime(CLOCK_MONOTONIC, &state.time);
	pthread_mutex_unlock(&mutex);

	usleep(1000*10);

	pthread_mutex_lock(&mutex);
	state.x++;
	state.y++;
	state.z++;
	clock_gettime(CLOCK_MONOTONIC, &state.time);
	pthread_mutex_unlock(&mutex);

}

void *read(void *thread)
{

	pthread_mutex_lock(&mutex);
	//clock_gettime(CLOCK_MONOTONIC, &state.time);
	double elapsedtime = (state.time.tv_sec*1000000000 + state.time.tv_nsec) - (start_time.tv_sec*1000000000 + start_time.tv_nsec);
	printf("state: %d, %d, %d, @ time since start: %lf nsec\n", state.x, state.y, state.z, elapsedtime);
	pthread_mutex_unlock(&mutex);

	usleep(10000);

	pthread_mutex_lock(&mutex);
	//clock_gettime(CLOCK_MONOTONIC, &state.time);
	elapsedtime = (state.time.tv_sec*1000000000 + state.time.tv_nsec) - (start_time.tv_sec*1000000000 + start_time.tv_nsec);
	printf("state: %d, %d, %d, @ time since start: %lf nsec\n", state.x, state.y, state.z, elapsedtime);
	pthread_mutex_unlock(&mutex);
}


int main (void)
{
	int ret;

	pthread_mutex_init(&mutex, NULL);

	clock_gettime(CLOCK_MONOTONIC, &start_time);
	double starttime = start_time.tv_sec*1000000000 + start_time.tv_nsec;
	printf("start state: %d, %d, %d, @ time: %lf nsec\n", state.x, state.y, state.z, starttime);
	
	ret = pthread_create(&thread1, NULL, update, NULL);
	if (ret != 0){perror("thread create fail");}

	ret = pthread_create(&thread2, NULL, read, NULL);
	if (ret != 0){perror("thread create fail");}

	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);

	printf("complete");

}


