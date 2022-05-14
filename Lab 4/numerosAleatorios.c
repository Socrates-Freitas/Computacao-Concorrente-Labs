#include <time.h> // Necessária para a geração de floats aleatórios
#include <stdlib.h> 
#include "numerosAleatorios.h"


void iniciarSeedAleatoria(){
    srand(time(NULL)); // inicia uma seed para floats aleatorios
}

// Retorna um float aleatorio entre 0 e 1
float randomFloat(){
    return (( (float)rand()) / ((float)RAND_MAX) );
}

float getRandomFloatFromInterval(float inicio, float fim){
    return randomFloat() * (fim - inicio) + inicio;
}

// int randomInt(){
//     return (( (int)rand()) / ((int)RAND_MAX) );
// }

int getRandomIntFromInterval(int inicio, int fim){
    return rand() % (fim - inicio) + inicio;
}

// int randomLongLongInt(){
//     return (( (long long int)rand()) / ((long long int)RAND_MAX) );
// }

int getRandomLongLongIntFromInterval(long long int inicio, long long int fim){
    return rand() % (fim - inicio) + inicio;
}
