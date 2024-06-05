#include <stdio.h>
#include <math.h>
#include <pthread.h>

#define THREADS 4
#define N 3000

int primes[N];
int pflag[N];
int total = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // 뮤텍스 설정
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;  // 조건 변수 설정
int done = 0;

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
            pthread_mutex_lock(&mutex);
            primes[total] = i;
            total++;
            pthread_mutex_unlock(&mutex);
        }
    }
    pthread_mutex_lock(&mutex);
    done++; // 작업을 완료한 스레드가 1 증가시킨다.
    if (done == THREADS) {
        pthread_cond_signal(&cond);
    }
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main(int argn, char **argv)
{
    int i;
    pthread_t tids[THREADS-1];
    int thread_ids[THREADS-1];
    
    for (i = 0; i < N; i++) {
        pflag[i] = 1; 
    }
    for (i = 0; i < THREADS-1; i++) {
        thread_ids[i] = i;
        pthread_create(&tids[i], NULL, work, (void *)&thread_ids[i]);
    }
    
    for (i = 0; i < THREADS-1; i++) {
        pthread_join(tids[i], NULL);
    }
    
    i = THREADS-1;
    work((void *)&i);
    
    pthread_mutex_lock(&mutex);
    while (done < THREADS) {
        pthread_cond_wait(&cond, &mutex);
    }
    pthread_mutex_unlock(&mutex);

    printf("Number of prime numbers between 2 and %d: %d\n",N, total);
    
    for (i = 0; i < total; i++) {
        printf("%d\n", primes[i]);
    }

    pthread_mutex_destroy(&mutex); // 뮤텍스 해제
    pthread_cond_destroy(&cond); // 조건 변수 해제
    return 0;
} 
