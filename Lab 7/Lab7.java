import java.util.ArrayList;
import java.util.Random;


class Retorno{
    private int resultado;
    public Retorno(){
        this.resultado = 0;
    }

    public synchronized void incrementarResultado(){
        this.resultado++;
    }

    public synchronized int getResultado(){
        return this.resultado;
    }
}

class Tarefa extends Thread{
    private int numThreads,tamanhoBloco, inicio, fim;
    private int numPares;
    private ArrayList<Integer> vetorNumeros;
    Retorno retorno; // variavel compartilhada

    public Tarefa(int numThreads,int threadId, ArrayList<Integer> vetor, Retorno retorno){

        this.numThreads = numThreads;
        this.tamanhoBloco = vetor.size() / this.numThreads;
        this.inicio = tamanhoBloco * threadId;
        this.fim = (threadId == numThreads-1)? vetor.size() : inicio+tamanhoBloco;
        this.numPares = 0;
        this.vetorNumeros = vetor;
        this.retorno = retorno;

    }

    public int getNumPares(){
        return numPares;
    }

    // executa tarefa
    public void run(){
        for(int i =inicio; i < fim;i++){
            if(vetorNumeros.get(i) % 2 == 0){
                this.retorno.incrementarResultado();
            }
        }

    }

}

class Lab7{
    public static void main(String[] args){

        int numThreads, tamanhoVetor;
        ArrayList<Thread> threads = new ArrayList<>();
        ArrayList<Integer> vetor;
        Retorno retorno = new Retorno();

        if(args.length < 2 ) {// se tem nao os argumentos
            System.out.println("Por favor, fornecer apenas <Tamanho do Vetor> <Numero de Threads>");
            System.exit(-1);
        }

        tamanhoVetor = Integer.parseInt(args[0]);
        vetor = new ArrayList<>( tamanhoVetor ); // pega o primeiro args
        numThreads = Integer.parseInt(args[1]); // pega o segundo args

        iniciarVetor(vetor, tamanhoVetor);

        for(int i = 0; i < numThreads; i++){
            threads.add(new Tarefa(numThreads,i,vetor, retorno));
        }

        for(int i = 0; i < numThreads; i++){
            threads.get(i).start();
        }

        for(int i =0; i < numThreads; i++){
            try{
                threads.get(i).join();
            }catch (InterruptedException e){
                System.out.println("Deu ruim no join");
            }
        }
        System.out.printf("Números pares: %d\n", retorno.getResultado());
    }

    public static void imprimirVetor(ArrayList<Integer> vetor){
        System.out.print("[");
        for(int i = 0; i < vetor.size(); i++){
            if(i != vetor.size() - 1){
                System.out.printf("%d, ", vetor.get(i));
            }
            else{
                System.out.printf("%d", vetor.get(i));
            }
        }
        System.out.println("]");
    }

    public static void iniciarVetor(ArrayList<Integer> vetor, int tamanho){
        int teto = 10000000;
        for(int i =0; i < tamanho; i++){
            Random random = new Random();
            vetor.add(random.nextInt(teto));
        }
    }

}
