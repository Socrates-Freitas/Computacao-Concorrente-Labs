# Lab 4


## Sobre o programa:

O programa `lab4.c` tem como entrada um vetor de tamanho *N*(fornecido na linha de comando) contendo números inteiros com valores aleatórios, então o programa irá retornar um vetor de números reais contendo as raízes quadradas dos números primos existentes no vetor de inteiros.



## Metodologia:

O programa `lab4.c` foi executado 5 vezes com 3 tamanhos de vetores  diferentes e 3 diferentes quantidades de threads para completar a tarefa. Após as 5 execuções, o *menor tempo* foi utilizado. 

O cálculo do *Ganho de desempenho* é a divisão do tempo de execução linear(*de N elementos*) pelo tempo de execução concorrente(*N elementos em T threads*). 



<br/>

# Tempos de execução:

## 10⁵ elementos

| Nº de Threads/ Tempo(s) | Sequencial | Concorrente | Ganho de Desempenho | 
|            :---:        |    :---:   |    :---:    |         :---:       | 
|            1            | 0.255621   | 0.257587    |    0.992367627      |
|            2            | 0.255621   | 0.137996    |    1.852379779      |
|            4            | 0.255621   | 0.115209    |    2.218758951      |


## 10⁷ elementos

| Nº de Threads/ Tempo(s) | Sequencial | Concorrente | Ganho de Desempenho | 
|            :---:        |    :---:   |    :---:    |         :---:       | 
|            1            | 26.088123  | 30.943281   |    0.843094919      |
|            2            | 26.088123  | 16.093595   |    1.621025197      |
|            4            | 26.088123  | 14.323654   |    1.821331554      |



## 10⁸ elementos

| Nº de Threads/ Tempo(s) | Sequencial | Concorrente | Ganho de Desempenho | 
|            :---:        |    :---:   |    :---:    |         :---:       | 
|            1            | 312.079890 | 329.196022  |      0.948006261    |
|            2            | 312.079890 | 170.310214  |      1.83242028     |
|            4            | 312.079890 | 142.299701  |      2.193116976    |

<br/>


## Especificações do computador utilizado para os testes acima:

<br/>

**Arquitetura:** x86_64

**Núcleos:** 4

**Memória RAM:** 8GB

**Modelo do processador:** Intel(R) Core(TM) i7-5500U CPU @ 2.40GHz


## Como compilar?

Preferencialmente em um sistema Linux, clone o repositório, então abra o terminal na pasta do projeto e digite `make`. Isto executará o comando no arquivo Makefile e irá compilar o projeto. 