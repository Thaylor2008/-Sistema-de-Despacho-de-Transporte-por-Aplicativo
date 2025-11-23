#ifndef TRECHO_H
#define TRECHO_H

class Parada;  // forward

enum TipoTrecho {
    COLETA,
    ENTREGA,
    DESLOCAMENTO
};

class Trecho {
private:
    Parada* inicio;
    Parada* fim;
    double distancia;
    double tempo;
    TipoTrecho tipo;
    
public:
    // construtor
    Trecho();
    Trecho(Parada* i, Parada* f, double dist, double temp, TipoTrecho t);

    Parada* getInicio();
    Parada* getFim();
    double getDistancia();
    double getTempo();
    TipoTrecho getTipo();
};
#endif