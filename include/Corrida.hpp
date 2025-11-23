#ifndef CORRIDA_H
#define CORRIDA_H
#include <iostream>
using std::ostream;

class Demanda;   // forward declaration
class Parada;    // forward
class Trecho;    // forward

class Corrida {
private:
    Demanda** demandas;
    int qtdDemandas;
    int capacidade;

    Parada** paradas;
    int qtdParadas;

    Trecho** trechos;
    int qtdTrechos;

    double distanciaTotal;
    double tempoTotal;
    double eficiencia;
    double tempoSolicitacao; // Tempo da demanda c_0
    double tempoConclusao;   // Tempo final absoluto da simulação

    friend ostream& operator<<(ostream& os, const Corrida& c);

public:
    // construtor
    Corrida(int capacidadeMax);

    void adicionarDemanda(Demanda* d);

    void gerarParadas();
    void gerarTrechos(double);

    bool cheio();
    double getDistanciaTotal();
    double getTempoTotal();
    double getEficiencia();
    int getQtdParadas();
    int getQtdDemandas();

    // Métodos necessários para o Escalonador (Fase 4/5)
    Parada* getParada(int index);   // Retorna a Parada no índice
    Trecho* getTrecho(int index);   // Retorna o Trecho no índice

    void setTempoSolicitacao(double t);
    double getTempoSolicitacao();

    void setTempoConclusao(double t);
    double getTempoConclusao();

    void imprimirSaidaFinal();

    double distancia(double, double, double, double);
    bool distancia_de_origens(Demanda* d, double);
    bool distancia_de_destinos(Demanda* d, double);

    void calcularDistanciaTotal();
    void calcularEficiencia();

    void removedemanda();

    // Destrutor
    ~Corrida();
};
#endif
