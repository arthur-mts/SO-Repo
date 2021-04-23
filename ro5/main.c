#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PROCESS 20
#define QUANTUM 5
#define BUFFER_SIZE 255

const char SEPARATOR[2] = ";";

typedef int FLAG_TYPE;
#define NOT_CONSUMED 0
#define CONSUMED 1

typedef struct
{
    char ID[5];
    int DURATION;
    FLAG_TYPE FLAG;

} Process;

Process processList[MAX_PROCESS];

void reorganizeList(Process *processList, int swapA, int size) {
    Process aux = processList[swapA];
    for(int i = swapA; i < size - 1; i ++)  {
        processList[i] = processList[i+1];
    }
    processList[size-1] = aux;
}

int main(int argc, char const *argv[])
{
    // Acessando o arquivo e populando a lista...
    FILE *file;

    Process p;

    char buffer[BUFFER_SIZE];

    char *value;

    int count = 0;

    file = fopen("./processList.txt", "r");

    while(fgets(buffer, BUFFER_SIZE, file)) {
        value = strtok(buffer, SEPARATOR);
        strcpy(p.ID, value);
        value = strtok(NULL, SEPARATOR);
        p.DURATION = atoi(value);
        p.FLAG = NOT_CONSUMED;
        processList[count] = p;
        count++;
    }

    fclose(file);

    int nonProcessedElements = count;
    int currentProcIndex = 0;

    Process *currentProc;

    while(nonProcessedElements > 0) {
        currentProc = &processList[currentProcIndex];
        printf("Processo corrente: %s\n", currentProc->ID);
        if(currentProc->FLAG == NOT_CONSUMED) {
            if(currentProc->DURATION <= QUANTUM){
                printf("Finalizando processo %s...\n", currentProc->ID);
                nonProcessedElements--;
                currentProcIndex = (currentProcIndex + 1) % count;
                currentProc->FLAG = CONSUMED;
                currentProc->DURATION = 0;
            }
            else {
                printf("Não foi possível completar o processo %s, chaveando novo processo...\n", currentProc->ID);
                currentProc->DURATION-=QUANTUM;
                reorganizeList(processList, currentProcIndex, count);
            }
        }
        else {
            currentProcIndex = (currentProcIndex + 1) % count;
        }
    }
    return 0;
}
