#include <stdio.h>
#include <math.h>
#include <pthread.h>
#include <semaphore.h>

#define THREADS 4
#define N 3000

int primes[N];
int pflag[N];
int total = 0;

sem_t *sem; // 세마포어 변수

int is_prime(int v)
{
    int i;
    int bound = floor(sqrt ((double)v)) + 1;
    for (i = 2; i < bound; i++) {
        /* No need to check against known composites */
        if (!pflag[i])
            continue;
        if (v % i == 0) {
            pflag[v] = 0;
            return 0;
        }
    }
    return (v > 1);
}

void *work(void *arg)
{
    int start;
    int end;
    int i;
    
    start = (((N/THREADS) * (*(int *)arg)))+2 ;
    end = (*(int *)arg == THREADS - 1) ? (N%2==0) ? N : N+1 : start + (N / THREADS);
    for (i = start; i < end; i++) {
        if (is_prime(i)) {
            sem_wait(sem); // 잠금
            primes[total] = i;
            total++;
            sem_post(sem); // 해제
        }
    }
    return NULL;
}

int main(int argn, char **argv)
{
    int i;
    pthread_t tids[THREADS-1];
    int thread_ids[THREADS-1];
    // 세마포어 할당
    if((sem = sem_open("/semaphore",O_CREAT,0644, 1)) == SEM_FAILED){
        return -1;
    }
    for (i = 0; i < N; i++) {
        pflag[i] = 1;
    }
    for (i = 0; i < THREADS-1; i++) {
        thread_ids[i] = i;
        pthread_create(&tids[i], NULL, work, (void *)&thread_ids[i]);
    }
    for (i = 0; i < THREADS-1; i++) {
        pthread_join(tids[i],NULL);
    }
    
    i = THREADS-1;
    work((void *)&i);
    
    printf("Number of prime numbers between 2 and %d: %d\n",
           N, total);
    
    for (i = 0; i < total; i++) {
        printf("%d\n", primes[i]);
    }
    
    sem_close(sem); // 해제
    
    return 0;
}
