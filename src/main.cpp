#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
using namespace std;

//classes 
#include "Demanda.hpp"
#include "Parada.hpp"
#include "Trecho.hpp"
#include "Corrida.hpp"
#include "heap.hpp"

const int EVENTO_COLETA_AGENDADA = 1; // 1ª coleta agendada, tempo = tempo da Demanda
const int EVENTO_CHEGADA_PARADA = 2;  // Veículo chegou a uma Parada (seja coleta ou entrega)

int main(){
    
    string entrada;
    cin >> entrada;
    
    ifstream arquivo(entrada);
    if (!arquivo.is_open()) {
        cout << "Erro ao abrir arquivo.\n";
        return 1;
    }

    // ================================
    // 1. Ler parâmetros da simulação
    // ================================
    int eta;         // capacidade do veículo
    double gama;     // velocidade
    double delta;    // intervalo de tempo p/ combinar
    double alfa;     // dist máx entre origens
    double beta;     // dist máx entre destinos
    double lambda;   // eficiência mínima
    int numDemandas; // número de demandas a serem simuladas
    double id, tempo, origemX, origemY, destinoX, destinoY;
    

    arquivo >> eta;
    arquivo >> gama;
    arquivo >> delta;
    arquivo >> alfa;
    arquivo >> beta;
    arquivo >> lambda;
    arquivo >> numDemandas;
    
    // ================================
    // 2. Ler demandas
    // ================================
    
    Demanda** pedidos = new Demanda*[numDemandas];

    for(int i = 0; i < numDemandas; i++){
        arquivo >> id >> tempo >> origemX >> origemY >> destinoX >> destinoY;

        pedidos[i] = new Demanda(id, tempo, origemX, origemY, destinoX, destinoY);
    }
    

    // ================================
    // 3. Criar e montar corridas
    // ================================
    
    int qtdCorridas = 0;  // começa sem nenhuma corrida criada
    Corrida** r;   // vetor de ponteiros para Corrida
    
    
    r = new Corrida*[numDemandas];
    r[qtdCorridas] = new Corrida(eta);
    
    for(int i = 0; i < numDemandas; i++){
        if(pedidos[i]->getCorrida() == nullptr){
            
           r[qtdCorridas]->adicionarDemanda(pedidos[i]);
           r[qtdCorridas]->gerarParadas();
           r[qtdCorridas]->gerarTrechos(gama);
           r[qtdCorridas]->calcularDistanciaTotal();
           r[qtdCorridas]->calcularEficiencia();
           
           for(int j = i+1; j < numDemandas; j++){
               double dt = pedidos[j]->getTempo() - pedidos[i]->getTempo();
               if (dt >= delta) break;
               
               
               if(r[qtdCorridas]->distancia_de_origens(pedidos[j], alfa)) break;
               if(r[qtdCorridas]->distancia_de_destinos(pedidos[j], beta)) break;
               
               
               if(r[qtdCorridas]->cheio()) break;
               
               r[qtdCorridas]->adicionarDemanda(pedidos[j]);
               r[qtdCorridas]->gerarParadas();
               r[qtdCorridas]->gerarTrechos(gama);
               r[qtdCorridas]->calcularDistanciaTotal();
               r[qtdCorridas]->calcularEficiencia();
               
               
               
               if(r[qtdCorridas]->getEficiencia() < lambda){
                   r[qtdCorridas]->removedemanda();
                   r[qtdCorridas]->gerarTrechos(gama);
                   r[qtdCorridas]->calcularDistanciaTotal();
                   r[qtdCorridas]->calcularEficiencia();
                   break;
               }
           }
           
           qtdCorridas++;
           r[qtdCorridas] = new Corrida(eta);
           
        }
    }

  
    // ================================
    // 4. Criar escalonador (min-heap)
    // ================================ 
    
    // A capacidade máxima é 2 * (capacidade máxima do veículo) * (número total de corridas)
    // Estimamos 2 * numDemandas, pois cada demanda gera 2 paradas
    MinHeap* escalonador = new MinHeap(numDemandas * 2 + 1); 

    // ================================
    // 5. Escalonamento Inicial
    // ================================   
    
    for (int i = 0; i < qtdCorridas; i++) {
        Corrida* r_atual = r[i];
        
        // Passo 1: Encontre a Demanda inicial (c_0) e sua Parada de Coleta (índice 0)
        // A parada inicial é sempre a de índice 0 no array de paradas da Corrida.
        Parada* primeira_parada = r_atual->getParada(0); 
        Demanda* c0 = primeira_parada->getDemanda(); // Pega a demanda c0
        // NOTA: Você precisará de um método getParada(int index) na classe Corrida.hpp/cpp
        // para acessar o array 'paradas' que está privado.
        // REGISTRO DO TEMPO DE SOLICITAÇÃO
        r_atual->setTempoSolicitacao(c0->getTempo());
        // Passo 2: Crie o evento
        Evento e;
        // O primeiro evento ocorre no tempo em que o cliente fez a solicitação (tempo da Demanda c_0)
        e.tempo = c0->getTempo();
        e.corrida = r_atual; 
        e.tipo = EVENTO_COLETA_AGENDADA;
        
        // Para rastrear a parada, podemos usar um campo extra no struct Evento,
        // ou criar um struct auxiliar. Aqui, vamos assumir que Parada tem um ID ou índice.
        // Vamos usar o campo 'tipo' para armazenar temporariamente o índice da parada (0)
        e.tipo = 0; // Usaremos 'tipo' para armazenar o índice da Parada no array r_atual->paradas
        
        // Passo 3: Insira no Escalonador
        escalonador->inserir(e);
        
        // Na prática, o evento escalonado é a "chegada do veículo na 1ª Parada de Coleta".
        // O tempo desse evento é o tempo da solicitação.
    }

    // ================================
    // 6. Loop Principal da Simulação (Fase 5)
    // ================================   
    
    double tempo_simulacao = 0;

    while (!escalonador->vazio()) {
        Evento e_atual = escalonador->extrairMin();
        
        tempo_simulacao = e_atual.tempo; // Avança o relógio de simulação
        Corrida* r_atual = (Corrida*)e_atual.corrida;
        int indice_parada_atual = e_atual.tipo; // Reutilizando 'tipo' como índice
        
        Parada* parada_atual = r_atual->getParada(indice_parada_atual); 

        // 1. Processamento do Evento (Coleta ou Entrega)
        
        // Lógica de Processamento (A ser detalhada)
        // ... (Aqui você marca a Demanda/Parada como atendida, etc.) ...

        // 2. Escalonamento do Próximo Evento
        int proximo_indice = indice_parada_atual + 1;
        
        if (proximo_indice >= r_atual->getQtdParadas()) {
            // Última Parada concluída: Gerar Estatísticas e Fim da Corrida
            // REGISTRO DO TEMPO DE CONCLUSÃO
            r_atual->setTempoConclusao(tempo_simulacao);
            r_atual->imprimirSaidaFinal(); // Necessário criar este método
            
        } else {
            // Agendar o próximo evento (chegada na próxima parada)
            
            // a. Encontre o Trecho que leva à próxima parada (índice_parada_atual)
            Trecho* trecho = r_atual->getTrecho(indice_parada_atual); // Necessário criar getTrecho(int)
            
            // b. Calcule o tempo de chegada
            double tempo_chegada_proxima = tempo_simulacao + trecho->getTempo();
            
            // c. Escalone o novo evento
            Evento e_proxima;
            e_proxima.tempo = tempo_chegada_proxima;
            e_proxima.corrida = r_atual;
            e_proxima.tipo = proximo_indice; // Novo índice da parada
            
            escalonador->inserir(e_proxima);
        }
    }
    
    // ================================
    // 7. Liberar memória (Fase 7)
    // ================================  

    // 1. Deletar o Escalonador (MinHeap)
    if (escalonador != nullptr) {
        delete escalonador;
        escalonador = nullptr;
    }
    
    // 2. Deletar as Corridas criadas (chamará o destrutor ~Corrida())
    for (int i = 0; i < qtdCorridas; i++) {
        if (r[i] != nullptr) {
            delete r[i];
            r[i] = nullptr;
        }
    }
    
    // 3. Deletar o array de ponteiros para Corridas
    if (r != nullptr) {
        delete[] r;
        r = nullptr;
    }

    // 4. Deletar as Demandas criadas (pedidos)
    for (int i = 0; i < numDemandas; i++) {
        if (pedidos[i] != nullptr) {
            delete pedidos[i];
            pedidos[i] = nullptr;
        }
    }

    // 5. Deletar o array de ponteiros para Demandas
    if (pedidos != nullptr) {
        delete[] pedidos;
        pedidos = nullptr;
    }

    return 0;
}