#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include "numerosAleatorios.h"
#include "timer.h"



// Globais

long long int dimensao;
int numThreads;
long long int *vetorAleatorio;
double *vetorPrimosProcessadosSequencial;
double *vetorPrimosProcessadosConcorrente;


#define TETO_NUMEROS_ALEATORIOS 100000000L // cem milões

// Variaveis para gerenciar threads
long long int i_global = 0;

// "chave"

pthread_mutex_t lock;

int ehPrimo(long long int n){
    if(n<=1) return 0;
    if(n==2) return 1;
    if(n%2==0) return 0;
    for(int i=3; i<sqrt(n)+1; i+=2)
        if(n%i==0) return 0;
    return 1;    
}



void imprimirVetor(double *vetor,long long int tamanho){
    if(tamanho == 0){
        puts("Vetor vazio!");
        return;
    }
    printf("tamanho: %lld\n",tamanho);
    printf("[");
    for(long long int i = 0; i < tamanho; i++){
        if(i != tamanho - 1){
            printf("%lf, ", vetor[i]);
        }
        else{
            printf("%lf",vetor[i]);
        }
    }
    printf("]\n");
}

void imprimirVetorInteiro(int *vetor,long long int tamanho){
    printf("tamanho: %lld\n",tamanho);
    printf("[");
    for(long long int i = 0; i < tamanho; i++){
        if(i != tamanho - 1){
            printf("%d, ", vetor[i]);
        }
        else{
            printf("%d",vetor[i]);
        }
    }
    printf("]\n");
}

void *processarRaizDePrimosConcorrente(void * arg){
    long long int i_local;
    
    pthread_mutex_lock(&lock);
    i_local = i_global;
    i_global++;

    pthread_mutex_unlock(&lock);

    while(i_local < dimensao){
        if(ehPrimo(vetorAleatorio[i_local])){
            vetorPrimosProcessadosConcorrente[i_local] = sqrt(vetorAleatorio[i_local]);
        }


        pthread_mutex_lock(&lock);
        i_local = i_global;
        i_global++;
        pthread_mutex_unlock(&lock);

    }
    


    pthread_exit(NULL);
}

void calcularRaizDePrimosSequencial(double *vetorPrimosSequencial, long long int tamanhoVetor){
    for(long long int i = 0; i < tamanhoVetor; i++){
        if(ehPrimo(vetorAleatorio[i])){ // se for primo
            vetorPrimosSequencial[i] = sqrt(vetorAleatorio[i]);
        }
    }
}


void compararVetores(){
  
    for(long long int i = 0; i < dimensao; i++ ){
        double itemSequencial = vetorPrimosProcessadosSequencial[i];
        double itemConcorrente = vetorPrimosProcessadosConcorrente[i];
        
        if(itemSequencial != itemConcorrente){
            printf("Item %lf(no vetor sequencial) é diferente de %lf(no vetor concorrente)\n\n",itemSequencial,itemConcorrente);
            return;
        }
    }
    puts("Os vetores são iguais!");
}

void iniciarVetorAleatorio(){
    for(long long int i = 0; i < dimensao; i++){
        vetorAleatorio[i] = getRandomLongLongIntFromInterval(0,TETO_NUMEROS_ALEATORIOS);
    }
}


int main(int argc, char *argv[]){

    // Inicializar estruturas de dados usadas pra threads(tid, args...)
    pthread_t *identificadores; 
    pthread_mutex_init(&lock,NULL); // inicializando o lock

    double inicio,fim,tempoSequencial, tempoConcorrente;


    iniciarSeedAleatoria();

    // tratamento de de entradas do usuario

    if(argc < 3){
        puts("Favor, fornecener apenas <Numero threads> <Tamanho do vetor>");
        exit(-1);
    }
    dimensao = atoll(argv[2]);
    numThreads = atoll(argv[1]);

    // Dar malloc nas coisas

    vetorAleatorio = (long long int*) malloc(dimensao * sizeof(long long int));
    if(vetorAleatorio == NULL){
        puts("Falha Malloc do vetor aleatorio!");
        exit(-1);
    }
    vetorPrimosProcessadosSequencial = (double*) malloc(dimensao * sizeof(double));
    if(vetorPrimosProcessadosSequencial == NULL){
        puts("Falha Malloc do vetor de primos processados sequencialmente!");
        exit(-1);
    }
    vetorPrimosProcessadosConcorrente = (double*) malloc(dimensao * sizeof(double));
    if(vetorPrimosProcessadosConcorrente == NULL){
        puts("Falha Malloc do vetor de primos processados concorrentemente!");
        exit(-1);
    }

    identificadores = (pthread_t*) malloc(numThreads * sizeof(pthread_t));
    if(identificadores == NULL){
        puts("Falha Malloc do vetor de identificadores de threads!");
        exit(-1);
    }

    iniciarVetorAleatorio();

    GET_TIME(inicio);

    calcularRaizDePrimosSequencial(vetorPrimosProcessadosSequencial,dimensao);
    
    GET_TIME(fim);
    tempoSequencial = fim - inicio;

    printf("Tempo de execução sequencial: %lf Segundos\n", tempoSequencial);

    // inicializar as threads

    GET_TIME(inicio);

    for(long long int i = 0; i < numThreads; i++){

        if(pthread_create(&identificadores[i],NULL,processarRaizDePrimosConcorrente, NULL )){
            puts("Erro no pthread_crete!");
            exit(-1);
        }
    }


    // Dar join
    for(long long int i = 0; i < numThreads; i++){
        if(pthread_join(identificadores[i],NULL)){
            puts("Erro no pthread_join!");
            exit(-1);
        }

    }

    GET_TIME(fim);

    tempoConcorrente = fim - inicio;
    printf("Tempo de execução concorrente(%d thread(s)): %lf Segundos\n\n",numThreads,tempoConcorrente);

    printf("Gannho de desempenho: %lf\n\n",tempoSequencial/tempoConcorrente);


    // destruindo o lock
    pthread_mutex_destroy(&lock);

    compararVetores();

    // Dar free nas coisas
    free(vetorAleatorio);
    free(vetorPrimosProcessadosSequencial);
    free(identificadores);

    pthread_exit(NULL);

    return 0;
}
