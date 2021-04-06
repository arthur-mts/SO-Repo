#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define NUMBER_OF_PHILO 5

// Definindo "semaforos" (mutex)
pthread_mutex_t table;

pthread_mutex_t FORKS[NUMBER_OF_PHILO];

// Definindo estados do programa
#define THINKING 0
#define HUNGRY 1
#define EATING 2

typedef int STATE;

STATE PHILOSOPHERS_STATE[NUMBER_OF_PHILO];

const clock_t MAXDELAY = 2000000;

void delay(clock_t ticks)
{ /* a "busy" delay */
    clock_t start = clock();
    do
        ;
    while (clock() < start + ticks);
}


void printState() {
    for(int l = 0; l < NUMBER_OF_PHILO ; l++){
        printf("PHILO %d: %d;   ", l, PHILOSOPHERS_STATE[l]);
    }
    printf("\n");
}

int getLeftIndex(int i)
{
    return (i + NUMBER_OF_PHILO - 1) % NUMBER_OF_PHILO;
}

int getRightIndex(int i)
{
    return (i + 1) % NUMBER_OF_PHILO;
}

void canTakeForks(int i)
{
    if ((PHILOSOPHERS_STATE[i] == HUNGRY) &&
        (PHILOSOPHERS_STATE[getLeftIndex(i)] != EATING) &&
         (PHILOSOPHERS_STATE[getRightIndex(i)] != EATING)
        )
    {
        printf("PEGOU O GARFO %d... \n", i);
        printf("A COMER... %d\n", i);
        PHILOSOPHERS_STATE[i] = EATING;
        pthread_mutex_unlock(&FORKS[i]);
    }
}

void take_forks(int i)
{
    pthread_mutex_lock(&table);
    printf("TENTOU PEGAR O GARFO %d\n", i);
    PHILOSOPHERS_STATE[i] = HUNGRY;
    
    canTakeForks(i);

    pthread_mutex_unlock(&table);
    pthread_mutex_lock(&FORKS[i]); //AQUI O FILOSOFO (SE ELE NAO PEGOU O GARFO)
    // DEVE ESPERAR ATE QUE ALGUEM PONHA OS GARFOS NA MESA
}

void put_fork(int i)
{
    pthread_mutex_lock(&table);
    if (PHILOSOPHERS_STATE[i] == EATING)
        printf("TERMINEI DE COMER %d\n", i);
    PHILOSOPHERS_STATE[i] = THINKING;
    canTakeForks(getLeftIndex(i));
    canTakeForks(getRightIndex(i));
    pthread_mutex_unlock(&table);
}

void eat(int i)
{
    delay(MAXDELAY);
}

void think(int i)
{
    delay(MAXDELAY * 2);
}

void *philosopher(int i)
{
    // int * i = (int *) (param);
    while (1)
    {
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

    //Inicio os semáforos (mutex)
    pthread_mutex_init(&table, NULL);
    for (int k = 0; k < NUMBER_OF_PHILO; k++)
        pthread_mutex_init(&FORKS[k], NULL);

    //Inicio as threads
    for (int k = 0; k < NUMBER_OF_PHILO; k++)
        pthread_create(&philo[k], NULL, philosopher, k);

    for (int k = 0; k < NUMBER_OF_PHILO; k++)
        pthread_join(philo[k], NULL);

    for (int k = 0; k < NUMBER_OF_PHILO; k++)
        pthread_mutex_destroy(&FORKS[k]);

    pthread_mutex_destroy(&table);
}