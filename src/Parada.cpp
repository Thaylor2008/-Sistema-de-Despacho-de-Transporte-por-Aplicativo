#include "Parada.hpp"

Parada::Parada() {}

Parada::Parada(double px, double py, TipoParada tipo_, Demanda* d){
    x = px;
    y = py;
    tipo = tipo_;
    demanda = d;
}

double Parada::getX(){ return x; }
double Parada::getY(){ return y; }
TipoParada Parada::getTipo(){ return tipo; }
Demanda* Parada::getDemanda(){ return demanda; }
