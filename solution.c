
#include <pthread.h>
#include <unistd.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <pthread.h>
#include <stdbool.h>

int numOfRandomNumbers;
int THREAD_COUNT;
long sum = 0;
long primeCounter = 0;
pthread_mutex_t lockmult;

int isPrime(int toCheck)
{

	if (toCheck <= 1) return 0; // small numbers are not primes
	if (toCheck <= 3) return 1; // 2 and 3 are prime
	if (toCheck % 2 == 0 || toCheck % 3 == 0) return 0; // multiples of 2 and 3 are not primes
	
	int currentIndex = 5;
	int max = sqrt(toCheck);


		while( currentIndex <= max) {
			if (toCheck % currentIndex == 0 || toCheck % (currentIndex+2) == 0) return 0;
			currentIndex += 6;
		}
		return 1;
	
	}
void *workers(void *work)
{
	long Tsum= 0;
	long Tcounter = 0;
	int *size = (int *)work;
	int random;
	int start= ((*size) * (numOfRandomNumbers / THREAD_COUNT));
	int end = ((*size + 1) * (numOfRandomNumbers / THREAD_COUNT));

	for (int i = start; i < end; i++)
	{
	       random=rand();
		
		if (isPrime(random)){
			Tsum += random;
			Tcounter++;
		}
}

	pthread_mutex_lock(&lockmult);
	sum += Tsum;
	primeCounter +=Tcounter;
	pthread_mutex_unlock(&lockmult);
	return NULL;
	}
	
	

void join_threads(pthread_t *threads)
{
    for(int i = 0; i < THREAD_COUNT; ++i)
        pthread_join(threads[i],NULL);
}

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		printf("Too few arguments ");
		printf("USAGE: ./primeCalc <prime pivot> <num of random numbers>");
		exit(0);
	}
       THREAD_COUNT = sysconf(_SC_NPROCESSORS_ONLN);
	srand(atoi(argv[1]));
	numOfRandomNumbers = atoi(argv[2]);
	

	pthread_t pthreads[THREAD_COUNT];


	for (int i = 0; i < THREAD_COUNT; i++)
	{
		pthread_create(&pthreads[i], NULL, workers, &i);
	}
	join_threads(pthreads);
	int final = numOfRandomNumbers % THREAD_COUNT;
	for(int i = 0; i < final; i++){
		int temp = rand();;
		if (isPrime(temp)){
			sum += temp;
			primeCounter++;
		}
	}

	//save this format
	printf("%ld,%ld\n", sum, primeCounter);
	pthread_exit(NULL);

	exit(0);
}
