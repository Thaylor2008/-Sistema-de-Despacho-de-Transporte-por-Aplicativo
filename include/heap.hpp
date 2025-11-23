#ifndef HEAP_H
#define HEAP_H

// Define a estrutura de um evento na simulação.
// O tempo é a chave de prioridade.
struct Evento {
    double tempo;      // Instante de ocorrência do evento (chave de prioridade)
    void* corrida;     // Ponteiro genérico para o objeto Corrida associado ao evento
    int tipo;          // Tipo de evento (ex: COLETA, ENTREGA, FIM_CORRIDA)
};

class MinHeap {
private:
    Evento* vet;      // Array para armazenar os eventos (implementação do heap)
    int tamanho;      // Número atual de elementos no heap
    int capacidade;   // Capacidade máxima do array

    // Funções auxiliares para manutenção da propriedade MinHeap
    void subir(int i);
    void descer(int i);

public:
    // Construtor e Destrutor (para liberar a memória de 'vet')
    MinHeap(int cap);
    ~MinHeap();

    // Funções da interface do Escalonador
    void inserir(Evento e);
    Evento extrairMin();
    bool vazio();
};

#endif