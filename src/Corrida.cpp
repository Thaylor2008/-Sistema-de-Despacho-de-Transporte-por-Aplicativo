#include "Corrida.hpp"
#include <cmath>
#include "Demanda.hpp"
#include "Parada.hpp"
#include "Trecho.hpp"
#include <iostream>
#include <iomanip>
using std::ostream;

Corrida::Corrida(int capacidadeMax){
    capacidade = capacidadeMax;
    demandas  = new Demanda*[capacidadeMax];
    paradas   = new Parada*[2 * capacidadeMax]; 
    trechos   = new Trecho*[2 * capacidadeMax];

    qtdDemandas = qtdParadas = qtdTrechos = 0;
    distanciaTotal = tempoTotal = eficiencia = 0;
    tempoSolicitacao = 0; // Inicializa
    tempoConclusao = 0;   // Inicializa
}

void Corrida::adicionarDemanda(Demanda* d) {
    if (qtdDemandas >= capacidade) {
        // Proteção: não ultrapassar capacidade
        return;
    }
    demandas[qtdDemandas++] = d;
    d->setCorrida(this);
}

void Corrida::gerarParadas(){
    // Antes de gerar novas paradas, liberar as antigas (se houver)
    if (qtdParadas > 0) {
        for (int i = 0; i < qtdParadas; i++) {
            if (paradas[i] != nullptr) {
                delete paradas[i];
                paradas[i] = nullptr;
            }
        }
    }

    qtdParadas = 0;
    // criar embarques
    for(int i = 0; i < qtdDemandas; i++){
        paradas[qtdParadas++] = new Parada(demandas[i]->getOX(), demandas[i]->getOY(), EMBARQUE, demandas[i]);
    }
    // criar desembarques
    for(int i = 0; i < qtdDemandas; i++){
        paradas[qtdParadas++] = new Parada(demandas[i]->getDX(), demandas[i]->getDY(), DESEMBARQUE, demandas[i]);
    }
}

void Corrida::gerarTrechos(double velocidade) {
    // Antes de gerar novos trechos, liberar os antigos (se houver)
    if (qtdTrechos > 0) {
        for (int i = 0; i < qtdTrechos; i++) {
            if (trechos[i] != nullptr) {
                delete trechos[i];
                trechos[i] = nullptr;
            }
        }
    }

    qtdTrechos = 0;

    for (int i = 0; i < qtdParadas - 1 ; i++) {
        Parada* A = paradas[i];
        Parada* B = paradas[i+1];

        double dist = distancia(A->getX(), A->getY(), B->getX(), B->getY());
        double temp = 0.0;
        if (velocidade > 0.0) temp = dist / velocidade;

        TipoTrecho tipo;

        if (A->getTipo() == EMBARQUE && B->getTipo() == EMBARQUE)
            tipo = COLETA;
        else if (A->getTipo() == DESEMBARQUE && B->getTipo() == DESEMBARQUE)
            tipo = ENTREGA;
        else
            tipo = DESLOCAMENTO;

        trechos[qtdTrechos++] = new Trecho(A, B, dist, temp, tipo);
    }
}

bool Corrida::cheio(){ 
    return qtdDemandas >= capacidade;
}
double Corrida::getDistanciaTotal(){ return distanciaTotal; }
double Corrida::getTempoTotal(){ return tempoTotal; }
double Corrida::getEficiencia(){ return eficiencia; }
int Corrida::getQtdParadas(){ return qtdParadas; }
int Corrida::getQtdDemandas(){ return qtdDemandas; }

Parada* Corrida::getParada(int index) {
    if (index >= 0 && index < qtdParadas) {
        return paradas[index];
    }
    return nullptr;
}

Trecho* Corrida::getTrecho(int index) {
    if (index >= 0 && index < qtdTrechos) {
        return trechos[index];
    }
    return nullptr;
}

void Corrida::setTempoSolicitacao(double t) { 
    tempoSolicitacao = t; 
}
double Corrida::getTempoSolicitacao() { 
    return tempoSolicitacao; 
}

void Corrida::setTempoConclusao(double t) { 
    tempoConclusao = t; 
}
double Corrida::getTempoConclusao() { 
    return tempoConclusao; 
}

void Corrida::imprimirSaidaFinal() {
    std::cout << std::fixed << std::setprecision(2);
    std::cout << tempoConclusao << " " << distanciaTotal << " " << qtdParadas;
    if (qtdParadas > 0) std::cout << " ";
    for(int i = 0; i < qtdParadas; i++) {
        Parada* p = getParada(i);
        std::cout << p->getX() << " " << p->getY();
        if (i < qtdParadas - 1) std::cout << " ";
    }
    std::cout << "\n";
}

double Corrida::distancia(double x1, double y1, double x2, double y2){
    double dx = x2 - x1;
    double dy = y2 - y1;
    return std::sqrt(dx*dx + dy*dy);
}

bool Corrida::distancia_de_origens(Demanda* d, double alfa){
    double teste = 0;
    for(int i= 0; i < qtdDemandas; i++){
        teste = distancia(demandas[i]->getOX(), demandas[i]->getOY(), d->getOX(), d->getOY());
        if(teste >= alfa){
            return true;
        }
    }
    return false;
}
bool Corrida::distancia_de_destinos(Demanda* d, double beta){
    double teste = 0;
    for(int i= 0; i < qtdDemandas; i++){
        teste = distancia(demandas[i]->getDX(), demandas[i]->getDY(), d->getDX(), d->getDY());
        if(teste >= beta){
            return true;
        }
    }
    return false;
}

void Corrida::calcularDistanciaTotal() {
    distanciaTotal = 0;
    tempoTotal = 0;

    for(int i = 0; i < qtdTrechos; i++){
        distanciaTotal += trechos[i]->getDistancia();
        tempoTotal += trechos[i]->getTempo();
    }
}
void Corrida::calcularEficiencia() {
    if(distanciaTotal == 0) {
        eficiencia = 0;
        return;
    }
    if(qtdDemandas == 1){
        eficiencia = 1;
        return;
    }

    double somaDistDireta = 0;

    for(int i = 0; i < qtdDemandas; i++){
        somaDistDireta += distancia(demandas[i]->getOX(), demandas[i]->getOY(), demandas[i]->getDX(), demandas[i]->getDY());
    }

    eficiencia = somaDistDireta / distanciaTotal;
}

void Corrida::removedemanda() {
    if (qtdDemandas == 0) return;

    // pegar última demanda
    Demanda* d = demandas[qtdDemandas - 1];

    // desconectar da corrida
    d->setCorrida(nullptr);

    // limpar posição
    demandas[qtdDemandas - 1] = nullptr;

    // atualizar contador
    qtdDemandas--;

    // Ao remover uma demanda precisamos reconstruir paradas e trechos:
    // antes, liberar paradas e trechos atuais para evitar vazamento
    if (qtdParadas > 0) {
        for (int i = 0; i < qtdParadas; i++) {
            if (paradas[i] != nullptr) {
                delete paradas[i];
                paradas[i] = nullptr;
            }
        }
    }
    if (qtdTrechos > 0) {
        for (int i = 0; i < qtdTrechos; i++) {
            if (trechos[i] != nullptr) {
                delete trechos[i];
                trechos[i] = nullptr;
            }
        }
    }

    // reconstruir
    qtdParadas = 0;
    qtdTrechos = 0;
    gerarParadas();
    // gerarTrechos NÃO pode ser chamado antes de gerarParadas()
    // gerarTrechos dependerá de passar a velocidade apropriada onde for chamado
}

Corrida::~Corrida() {
    // Deletar todos os objetos Parada
    if (paradas != nullptr) {
        for (int i = 0; i < qtdParadas; i++) {
            if (paradas[i] != nullptr) {
                delete paradas[i];
                paradas[i] = nullptr;
            }
        }
    }

    // Deletar todos os objetos Trecho
    if (trechos != nullptr) {
        for (int i = 0; i < qtdTrechos; i++) {
            if (trechos[i] != nullptr) {
                delete trechos[i];
                trechos[i] = nullptr;
            }
        }
    }

    // 2. Liberar os arrays de ponteiros alocados no construtor.
    // Os objetos Demanda são gerenciados pela 'main', então não são deletados aqui.
    if (demandas != nullptr) {
        delete[] demandas;
        demandas = nullptr;
    }
    if (paradas != nullptr) {
        delete[] paradas;
        paradas = nullptr;
    }
    if (trechos != nullptr) {
        delete[] trechos;
        trechos = nullptr;
    }
}

// sobrecarga de operador <<
ostream& operator<<(ostream& os, const Corrida& c) {
    os << "===== Corrida =====\n";
    os << "Qtd demandas: " << c.qtdDemandas << "\n";
    os << "Qtd paradas: " << c.qtdParadas << "\n";
    os << "Qtd trechos: " << c.qtdTrechos << "\n";
    os << "Capacidade: " << c.capacidade << "\n";

    if (c.qtdDemandas == 0) {
        os << "(corrida vazia)\n";
        return os;
    }

    os << "\n-- Demandas --\n";
    for (int i = 0; i < c.qtdDemandas; i++) {
        Demanda* d = c.demandas[i];
        os << "Demanda " << d->getId()
           << " | t=" << d->getTempo()
           << " | O=(" << d->getOX() << "," << d->getOY() << ")"
           << " | D=(" << d->getDX() << "," << d->getDY() << ")\n";
    }

    os << "\n-- Eficiencia --\n";
    os << c.eficiencia << "\n";

    os << "====================\n";

    return os;
}
