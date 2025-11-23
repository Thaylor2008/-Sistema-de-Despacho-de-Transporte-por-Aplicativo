#ifndef PARADA_H
#define PARADA_H

class Demanda;

enum TipoParada {
    EMBARQUE,
    DESEMBARQUE
};

class Parada {
private:

    double x, y;
    TipoParada tipo;
    Demanda* demanda;
    
public:
    // construtor
    Parada();
    Parada(double px, double py, TipoParada tipo_, Demanda* d);

    double getX();
    double getY();
    TipoParada getTipo();
    Demanda* getDemanda();
};
#endif