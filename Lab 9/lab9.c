#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define NUMTHREADS 5

// Globais
sem_t semaforoT1,semaforoT24;
int estado_dialogo = 0;

void *tarefaThread234(void *arg){ // intermediários
    char *textoArgumento = (char *) arg; 

    sem_wait(&semaforoT24);

    puts(textoArgumento);
    estado_dialogo++;
     if(estado_dialogo == 4){
       sem_post(&semaforoT1);
    }
    
    pthread_exit(NULL);//
}

void *tarefaThread1(void *arg){ // aqui executa por último
    char *textoArgumento = (char *) arg;

    sem_wait(&semaforoT1);    
   
    puts(textoArgumento);

    pthread_exit(NULL);
}

void *tarefaThread5(void *arg){ // aqui executa primeiro
    char *textoArgumento = (char *) arg;

    puts(textoArgumento);

    estado_dialogo++;
    for(int i = 0; i < 3; i++)  {
        sem_post(&semaforoT24);
    }

    pthread_exit(NULL);
}

int main(){
    char *frases[5] = {"Volte sempre!","Fique a vontade.","Sente-se por favor.",
    "Aceita um copo d' água?","Seja bem vindo!"};

    pthread_t identificadores[NUMTHREADS];

    sem_init(&semaforoT24,0,0);
    sem_init(&semaforoT1,0,0);


    pthread_create(&identificadores[0],NULL,tarefaThread1,(void *) frases[0]); // iniciando thread 1
    
    for(int i = 1; i < NUMTHREADS - 1; i++){ // iniciando threads 2-4
        pthread_create(&identificadores[i],NULL, tarefaThread234, (void *) frases[i]);
    }
   
    pthread_create(&identificadores[4],NULL,tarefaThread5,(void *) frases[NUMTHREADS - 1] ); // iniciando thread 5

    for(int i = 0; i < NUMTHREADS; i++){
        pthread_join(identificadores[i],NULL);
    }
    
    sem_destroy(&semaforoT1);
    sem_destroy(&semaforoT24);

    pthread_exit(NULL);
    return 0;
}
