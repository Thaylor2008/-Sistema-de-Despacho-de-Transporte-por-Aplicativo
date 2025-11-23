#include "heap.hpp"
#include <iostream>
#include <algorithm> // Necessário para a função std::swap, que pode ser usada em substituição à implementação manual

// Construtor: Aloca o array de eventos dinamicamente
MinHeap::MinHeap(int cap) {
    capacidade = cap;
    tamanho = 0;
    // O heap é 1-baseado (índice 0 não usado), então alocamos cap + 1
    vet = new Evento[capacidade + 1]; 
}

// Destrutor (Essencial para a Fase 7 - Memória)
MinHeap::~MinHeap() {
    if (vet != nullptr) {
        delete[] vet;
        vet = nullptr;
    }
}

// Verifica se o heap está vazio
bool MinHeap::vazio() {
    return tamanho == 0;
}

// Função auxiliar para manter a propriedade MinHeap ao inserir
void MinHeap::subir(int i) {
    // Enquanto não for a raiz (i > 1) e o nó for menor que seu pai
    while (i > 1 && vet[i].tempo < vet[i / 2].tempo) {
        // Troca o nó com o seu pai
        std::swap(vet[i], vet[i / 2]);
        i = i / 2;
    }
}

// Função auxiliar para manter a propriedade MinHeap ao extrair
void MinHeap::descer(int i) {
    int menor;
    int esq = 2 * i;
    int dir = 2 * i + 1;

    // Compara o nó com o filho esquerdo
    if (esq <= tamanho && vet[esq].tempo < vet[i].tempo) {
        menor = esq;
    } else {
        menor = i;
    }

    // Compara o resultado com o filho direito
    if (dir <= tamanho && vet[dir].tempo < vet[menor].tempo) {
        menor = dir;
    }

    // Se o menor não for o nó atual (i), troca e desce
    if (menor != i) {
        std::swap(vet[i], vet[menor]);
        descer(menor);
    }
}

// Insere um novo evento no heap
void MinHeap::inserir(Evento e) {
    if (tamanho >= capacidade) {
        // Idealmente, realocaria o array, mas para o TP,
        // pode-se apenas ignorar ou emitir um erro se a capacidade for excedida.
        // Assumindo que a capacidade alocada é suficiente.
        return; 
    }

    tamanho++;
    vet[tamanho] = e;
    subir(tamanho);
}

// Remove e retorna o evento de menor tempo (o próximo a ser processado)
Evento MinHeap::extrairMin() {
    if (vazio()) {
        // Retornar um evento vazio ou lançar uma exceção
        return Evento{0.0, nullptr, 0}; 
    }

    // O elemento mínimo é sempre a raiz (índice 1)
    Evento min_evento = vet[1];

    // Move o último elemento para a raiz
    vet[1] = vet[tamanho];
    tamanho--;

    // Restaura a propriedade MinHeap
    descer(1);

    return min_evento;
}

