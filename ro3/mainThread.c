    #include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define NUMBER_OF_PHILO 5

// Definindo "semaforos" (mutex)
pthread_mutex_t table;

pthread_cond_t PHILOSOPHERS_ACCESS[NUMBER_OF_PHILO];

// Definindo estados do programa
#define THINKING 0 
#define HUNGRY 1
#define EATING 2

typedef int STATE;

STATE PHILOSOPHERS_STATE[NUMBER_OF_PHILO];

const clock_t MAXDELAY = 2000000;


void delay(clock_t ticks) { /* a "busy" delay */
  clock_t start = clock();
  do
    ; while (clock() < start + ticks);
}


int getLeftIndex(int i) {
    return (i+NUMBER_OF_PHILO-1)%NUMBER_OF_PHILO;
}

int getRightIndex(int i) {
    return (i+ 1)%NUMBER_OF_PHILO;
}

void canTakeForks(int i) {
    if(PHILOSOPHERS_STATE[i] == HUNGRY && 
        PHILOSOPHERS_STATE[getLeftIndex(i)] != EATING && 
        PHILOSOPHERS_STATE[getRightIndex(i)] != EATING){
            printf("PEGUEI O GARFO %d \n", i);
            PHILOSOPHERS_STATE[i] = EATING;
            // pthread_cond_wait(&PHILOSOPHERS_ACCESS[i], &table);
            pthread_cond_signal(&PHILOSOPHERS_ACCESS[i]);
            // pthread_cond_signal(&PHILOSOPHERS_ACCESS[getLeftIndex(i)]);
        }   
            // pthread_cond_wait(&PHILOSOPHERS_ACCESS[getLeftIndex(i)], &table);
            // pthread_cond_wait(&PHILOSOPHERS_ACCESS[getRightIndex(i)], &table);
    else{
        printf("TENTOU PEGAR O GARFO E NÃO CONSEGUIU %d\n", i);
        pthread_cond_wait(&PHILOSOPHERS_ACCESS[i], &table);
    } 
    
}

void take_forks(int i) {
    pthread_mutex_lock(&table);
    PHILOSOPHERS_STATE[i] = HUNGRY;
    canTakeForks(i);
    pthread_mutex_unlock(&table);
    // pthread_cond_wait(&PHILOSOPHERS_ACCESS[i], &table);
}

void put_fork(int i) {
    printf("A TERMINAR DE COMER... %d\n", i);
    pthread_mutex_lock(&table);
    PHILOSOPHERS_STATE[i] = THINKING;
    canTakeForks(getRightIndex(i));
    canTakeForks(getLeftIndex(i));
    // pthread_cond_signal(&PHILOSOPHERS_ACCESS[getLeftIndex(i)]);
    // pthread_cond_signal(&PHILOSOPHERS_ACCESS[getRightIndex(i)]);
    pthread_mutex_unlock(&table);
}

void eat(int i) {
    printf("A COMER... %d\n", i);
    delay(MAXDELAY/2);
}

void think(int i) {
    printf("A PENSAR %d...\n", i);
    delay(MAXDELAY);    
}

void *philosopher(void * param) {
    int * i = (int *) (param);
    
    while(1) {
        think(i);
        take_forks(i);
        eat(i);
        put_fork(i);
    }
    pthread_exit(0);
}


int main(int argc, char **argv)
{
    pthread_t philo[NUMBER_OF_PHILO];

    pthread_mutex_init(&table, 0);


    for(int k = 0; k < NUMBER_OF_PHILO; k++) {
        pthread_create(&philo[k], 0, philosopher, k);
        pthread_cond_init(&PHILOSOPHERS_ACCESS[k], 0);
    }

    for(int k = 0; k < NUMBER_OF_PHILO; k++) pthread_join(philo[k], 0);

    for(int k = 0; k < NUMBER_OF_PHILO; k++) pthread_cond_destroy(&PHILOSOPHERS_ACCESS[k]);

    pthread_mutex_destroy(&table);

}