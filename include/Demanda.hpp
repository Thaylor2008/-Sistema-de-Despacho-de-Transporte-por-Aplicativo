#ifndef DEMANDA_H
#define DEMANDA_H
#include <iostream>

class Corrida;
class Demanda;

// sobrecarga de operador << 
std::ostream& operator<<(std::ostream& os, const Demanda& d);


enum EstadoDemanda {
    DEMANDADA,
    INDIVIDUAL,
    COMBINADA,
    CONCLUIDA
};

class Demanda {
private:
    int id;
    double tempo;
    double oX, oY;
    double dX, dY;
    
    EstadoDemanda estado;
    
    class Corrida* corrida; // ponteiro pra corrida associada
    
    friend std::ostream& operator<<(std::ostream& os, const Demanda& d);
    
public:
    // construtor
    Demanda();
    Demanda(int id, double t, double ox, double oy, double dx, double dy);

    int getId();
    double getTempo();
    double getOX();
    double getOY();
    double getDX();
    double getDY();

    EstadoDemanda getEstado();
    void setEstado(EstadoDemanda e);

    void setCorrida(Corrida* c);
    Corrida* getCorrida();

};  
#endif