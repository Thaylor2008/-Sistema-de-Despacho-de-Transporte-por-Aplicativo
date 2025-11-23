#include "Demanda.hpp"


// sobrecarga de operador 
std::ostream& operator<<(std::ostream& os, const Demanda& d) {
    os << "ID=" << d.id
       << " Tempo=" << d.tempo
       << " Origem=(" << d.oX << "," << d.oY << ")"
       << " Destino=(" << d.dX << "," << d.dY << ")";
    return os;
}


Demanda::Demanda() {}

Demanda::Demanda(int id_, double t, double ox, double oy, double dx, double dy) {
    id = id_;
    tempo = t;
    oX = ox;
    oY = oy;
    dX = dx;
    dY = dy;
    estado = DEMANDADA;
    corrida = nullptr;
}

int Demanda::getId() { return id; }
double Demanda::getTempo() { return tempo; }
double Demanda::getOX() { return oX; }
double Demanda::getOY() { return oY; }
double Demanda::getDX() { return dX; }
double Demanda::getDY() { return dY; }

EstadoDemanda Demanda::getEstado(){ return estado; }
void Demanda::setEstado(EstadoDemanda e){ estado = e; }

void Demanda::setCorrida(Corrida* c){ corrida = c; }
Corrida* Demanda::getCorrida(){ return corrida; }
