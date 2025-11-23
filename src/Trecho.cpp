#include "Trecho.hpp"

Trecho::Trecho() {}

Trecho::Trecho(Parada* i, Parada* f, double dist, double temp, TipoTrecho t){
    inicio = i;
    fim = f;
    distancia = dist;
    tempo = temp;
    tipo = t;
}

Parada* Trecho::getInicio(){ return inicio; }
Parada* Trecho::getFim(){ return fim; }
double Trecho::getDistancia(){ return distancia; }
double Trecho::getTempo(){ return tempo; }
TipoTrecho Trecho::getTipo(){ return tipo; }
