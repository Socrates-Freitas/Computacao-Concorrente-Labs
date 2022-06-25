import java.util.ArrayList;

class Monitor{
    private int escritores;
    private int leitores;


    public Monitor() {
        this.escritores=0;
        this.leitores = 0;
    }

    synchronized void entraLeitor(int threadId, String nomeThread){
        try {
            while (this.escritores > 0) { // tem alguem escrevendo?
                wait();
                System.out.printf("Leitor(%s) %d bloqueado\n",nomeThread, threadId);
            }
            this.leitores++;
           // System.out.printf("Leitor(%s) %d desbloqueado\n",nomeThread, threadId);

            System.out.printf("Leitor(%s) %d lendo\n",nomeThread, threadId);

        }catch(InterruptedException e){}
    }

    synchronized void saiLeitor(int threadId, String nomeThread){
        this.leitores--;
        System.out.printf("Leitor(%s) %d saindo\n",nomeThread, threadId);
        if(this.leitores == 0){
            notifyAll();
        }
    }

  
    synchronized void entraEscritor(int threadId, String nomeThread){
        try{
            while(this.leitores > 0 || this.escritores > 0){ // tem alguem lendo ou escrevendo?
                System.out.printf("Escritor(%s) %d bloqueado\n",nomeThread, threadId);
                wait();
            }
            this.escritores++;
           // System.out.printf("Escritor(%s) %d desbloqueado\n",nomeThread, threadId);

            System.out.printf("Escritor(%s) %d escrevendo\n",nomeThread, threadId);

        }catch(InterruptedException e){}
    }

    synchronized void saiEscritor(int threadId, String nomeThread){
        this.escritores--;
        System.out.printf("Escritor(%s) %d saindo\n",nomeThread, threadId);
        notifyAll();
    }


}

class Compartilhada{ // Variavel Compartilhada
    private int valor;

    public Compartilhada() {
        this.valor = 0;
    }

    public int getValor() {
        return valor;
    }

    public void incValor(){
        this.valor++;
    }

    public void setValor(int valor) {
        this.valor = valor;
    }
}

class T1 extends Thread{
    private Monitor monitor;
    private Compartilhada compartilhada; // variavel compartilhada
    private String nomeThread;
    private int threadId;

    public T1(Monitor monitor,Compartilhada compartilhada, int threadId) {
        this.monitor = monitor;
        this.compartilhada = compartilhada;
        this.threadId = threadId;
        this.nomeThread = "T1";
    }

    @Override
    public void run() {
        super.run();

        while(true) {
            this.monitor.entraEscritor(this.threadId,this.nomeThread);

            this.compartilhada.incValor();

            this.monitor.saiEscritor(this.threadId,this.nomeThread);
        }
    }
}

class T2 extends Thread{ // Thread que vai ler e verificar se O valor é par ou ímpar
    private Monitor monitor;
    private int threadId;
    private Compartilhada compartilhada;
    private String nomeThread;

    public T2(Monitor monitor, Compartilhada compartilhada,int threadId){
        this.monitor = monitor;
        this.compartilhada = compartilhada;
        this.threadId = threadId;
        this.nomeThread = "T2";
    }

    @Override
    public void run() {
        super.run();

        int valorVariavelCompartilhada = 0;

        while(true) {

            this.monitor.entraLeitor(this.threadId, this.nomeThread);
            
            valorVariavelCompartilhada = this.compartilhada.getValor();

            if (valorVariavelCompartilhada % 2 == 0) { // Se for par...
                System.out.printf("%d é par!\n", valorVariavelCompartilhada);
            } else {
                System.out.printf("%d é ímpar!\n", valorVariavelCompartilhada);
            }

            this.monitor.saiLeitor(this.threadId, this.nomeThread);
        }

    }
}

class T3 extends Thread{
    private Monitor monitor;
    private Compartilhada compartilhada;
    private int threadId;
    private String nomeThread;

    public T3(Monitor monitor, Compartilhada compartilhada, int threadId) {
        this.monitor = monitor;
        this.compartilhada = compartilhada;
        this.threadId = threadId;
        this.nomeThread = "T3";
    }

    @Override
    public void run() {
        super.run();


        int valorVariavelCompartilhada;

        while(true) {

            this.monitor.entraLeitor(this.threadId, this.nomeThread);

            valorVariavelCompartilhada = this.compartilhada.getValor();

            System.out.println("Valor compartilhado: " + valorVariavelCompartilhada);

            this.monitor.saiLeitor(this.threadId, this.nomeThread);

            try {
                sleep(200L);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }

            this.monitor.entraEscritor(this.threadId, this.nomeThread);

            this.compartilhada.setValor(this.threadId);

            this.monitor.saiEscritor(this.threadId, this.nomeThread);



        }
    }
}

public class Lab8 {

    static final int numT1 = 1;
    static final int numT2 = 1;
    static final int numT3 = 1;



    public static void main(String[] args) {
        ArrayList<T1> thredsT1 = new ArrayList<>();
        ArrayList<T2> thredsT2 = new ArrayList<>();
        ArrayList<T3> thredsT3 = new ArrayList<>();

        Compartilhada compartilhada = new Compartilhada();
        Monitor monitor = new Monitor();

        // write your code here
        for(int i = 0; i < numT1; i++){
            thredsT1.add( new T1(monitor,compartilhada,i+1) );
            thredsT1.get(i).start();
        }

        for(int i = 0; i < numT2; i++){
            thredsT2.add( new T2(monitor,compartilhada,i+1) );
            thredsT2.get(i).start();
        }

        for(int i = 0; i < numT3; i++){
            thredsT3.add( new T3(monitor,compartilhada,i+1) );
            thredsT3.get(i).start();
        }
    }
}
