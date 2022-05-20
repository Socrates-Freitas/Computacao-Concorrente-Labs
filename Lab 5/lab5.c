#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUMTHREADS 5

// Globais
pthread_mutex_t lock;
pthread_cond_t cond, condVolteSempre; 
int estado_dialogo = 0;

void *tarefaThread234(void *arg){ // intermediários
    char *textoArgumento = (char *) arg; 
    pthread_mutex_lock(&lock);

     while(estado_dialogo < 1){
        // bloqueando a thread
        pthread_cond_wait(&cond,&lock); 
    }

    puts(textoArgumento);
    estado_dialogo++;
     if(estado_dialogo == 4){
        pthread_cond_signal(&condVolteSempre);
    }

    pthread_mutex_unlock(&lock);

    pthread_exit(NULL);
}

void *tarefaThread1(void *arg){ // aqui executa por último
    char *textoArgumento = (char *) arg;

    pthread_mutex_lock(&lock);
    
    while(estado_dialogo < 4){
        pthread_cond_wait(&condVolteSempre,&lock);
    }
    puts(textoArgumento);
  
    pthread_mutex_unlock(&lock); 
    pthread_exit(NULL);
}

void *tarefaThread5(void *arg){ // aqui executa primeiro
    char *textoArgumento = (char *) arg;

    pthread_mutex_lock(&lock);
    puts(textoArgumento);

    estado_dialogo++;
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&lock);

    pthread_exit(NULL);
}

int main(){
    char *frases[5] = {"Volte sempre!","Fique a vontade.","Sente-se por favor.",
    "Aceita um copo d' água?","Seja bem vindo!"};

    pthread_t identificadores[NUMTHREADS];

    pthread_mutex_init(&lock,NULL);
    pthread_cond_init(&cond,NULL);
    pthread_cond_init(&condVolteSempre,NULL);

    pthread_create(&identificadores[0],NULL,tarefaThread1,(void *) frases[0]); // iniciando thread 1
    
    for(int i = 1; i < 4; i++){ // iniciando threads 2-4
        pthread_create(&identificadores[i],NULL, tarefaThread234, (void *) frases[i]);
    }
   
    pthread_create(&identificadores[4],NULL,tarefaThread5,(void *) frases[4] ); // iniciando thread 5

    for(int i = 0; i < NUMTHREADS; i++){
        pthread_join(identificadores[i],NULL);
    }
    
    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&cond);
    pthread_cond_destroy(&condVolteSempre);

    pthread_exit(NULL);
    return 0;
}
