#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <sstream>
#include "corretor.h"
#include "cliente.h"
#include "imovel.h"

constexpr double EARTH_R = 6371.0; // Raio da Terra em km

// Função Haversine para calcular distância entre coordenadas
double haversine(double lat1, double lon1, double lat2, double lon2) {
    auto deg2rad = [](double d){ return d * M_PI / 180.0; };
    double dlat = deg2rad(lat2 - lat1);
    double dlon = deg2rad(lon2 - lon1);
    double a = std::pow(std::sin(dlat/2), 2) +
               std::cos(deg2rad(lat1)) * std::cos(deg2rad(lat2)) *
               std::pow(std::sin(dlon/2), 2);
    double c = 2 * std::atan2(std::sqrt(a), std::sqrt(1 - a));
    return EARTH_R * c;
}

// Estrutura para armazenar agendamentos
struct Agendamento {
    int hora;
    int minuto;
    int imovelId;
    
    Agendamento(int h, int m, int id) : hora(h), minuto(m), imovelId(id) {}
};

// Função para converter minutos desde 09:00 para hora e minuto
std::pair<int, int> minutosParaHoraMinuto(int minutos) {
    int totalMinutos = 9 * 60 + minutos; // 09:00 = 540 minutos
    int hora = totalMinutos / 60;
    int minuto = totalMinutos % 60;
    return {hora, minuto};
}

// Função para encontrar o imóvel mais próximo não visitado
int encontrarImovelMaisProximo(const std::vector<Imovel>& imoveis, 
                               const std::vector<int>& imoveisCorretor,
                               const std::vector<bool>& visitados,
                               double latAtual, double lonAtual) {
    int melhorImovel = -1;
    double menorDistancia = std::numeric_limits<double>::max();
    
    for (int imovelId : imoveisCorretor) {
        if (!visitados[imovelId - 1]) { // IDs começam em 1, índices em 0
            double distancia = haversine(latAtual, lonAtual, 
                                       imoveis[imovelId - 1].latitude, 
                                       imoveis[imovelId - 1].longitude);
            if (distancia < menorDistancia) {
                menorDistancia = distancia;
                melhorImovel = imovelId;
            }
        }
    }
    
    return melhorImovel;
}

// Função para gerar agendamento para um corretor
std::vector<Agendamento> gerarAgendamentoCorretor(const std::vector<Imovel>& imoveis,
                                                  const Corretor& corretor) {
    std::vector<Agendamento> agendamentos;
    std::vector<bool> visitados(imoveis.size(), false);
    
    double latAtual = corretor.latitude;
    double lonAtual = corretor.longitude;
    int tempoAtual = 0; // minutos desde 09:00
    
    int imoveisRestantes = corretor.imoveisAtribuidos.size();
    
    while (imoveisRestantes > 0) {
        int proximoImovel = encontrarImovelMaisProximo(imoveis, corretor.imoveisAtribuidos, 
                                                       visitados, latAtual, lonAtual);
        
        if (proximoImovel == -1) break; // Não deveria acontecer
        
        // Calcular tempo de deslocamento
        double distancia = haversine(latAtual, lonAtual, 
                                   imoveis[proximoImovel - 1].latitude, 
                                   imoveis[proximoImovel - 1].longitude);
        int tempoDeslocamento = static_cast<int>(distancia * 2); // 2 min/km, truncar em vez de arredondar
        
        // Atualizar tempo atual
        tempoAtual += tempoDeslocamento;
        
        // Criar agendamento
        auto [hora, minuto] = minutosParaHoraMinuto(tempoAtual);
        agendamentos.emplace_back(hora, minuto, proximoImovel);
        
        // Atualizar posição atual
        latAtual = imoveis[proximoImovel - 1].latitude;
        lonAtual = imoveis[proximoImovel - 1].longitude;
        
        // Marcar como visitado
        visitados[proximoImovel - 1] = true;
        imoveisRestantes--;
        
        // Adicionar 60 minutos da avaliação
        tempoAtual += 60;
    }
    
    return agendamentos;
}

int main() {
    std::vector<Corretor> corretores;
    std::vector<Cliente> clientes;
    std::vector<Imovel> imoveis;
    
    // Ler corretores
    int numCorretores;
    std::cin >> numCorretores;
    
    for (int i = 0; i < numCorretores; ++i) {
        std::string telefone;
        int avaliador;
        double lat, lon;
        std::cin >> telefone >> avaliador >> lat >> lon;
        
        std::string nome;
        std::getline(std::cin >> std::ws, nome);
        
        corretores.emplace_back(telefone, avaliador == 1, lat, lon, nome);
    }
    
    // Ler clientes
    int numClientes;
    std::cin >> numClientes;
    
    for (int i = 0; i < numClientes; ++i) {
        std::string telefone;
        std::cin >> telefone;
        
        std::string nome;
        std::getline(std::cin >> std::ws, nome);
        
        clientes.emplace_back(telefone, nome);
    }
    
    // Ler imóveis
    int numImoveis;
    std::cin >> numImoveis;
    
    for (int i = 0; i < numImoveis; ++i) {
        std::string tipo;
        int propId;
        double lat, lon, preco;
        std::cin >> tipo >> propId >> lat >> lon >> preco;
        
        std::string endereco;
        std::getline(std::cin >> std::ws, endereco);
        
        imoveis.emplace_back(Imovel::stringToTipo(tipo), propId, lat, lon, preco, endereco);
    }
    
    // Filtrar apenas corretores avaliadores
    std::vector<Corretor*> avaliadores;
    for (auto& corretor : corretores) {
        if (corretor.isAvaliador()) {
            avaliadores.push_back(&corretor);
        }
    }
    
    // Distribuir imóveis (Round-Robin)
    // Ordenar imóveis por ID (já estão ordenados pela criação sequencial)
    for (int i = 0; i < numImoveis; ++i) {
        int avaliadorIndex = i % avaliadores.size();
        avaliadores[avaliadorIndex]->adicionarImovel(imoveis[i].id);
    }
    
    // Gerar e imprimir agendamentos
    bool primeiro = true;
    for (const auto& corretor : corretores) {
        if (corretor.isAvaliador() && !corretor.imoveisAtribuidos.empty()) {
            if (!primeiro) {
                std::cout << std::endl;
            }
            primeiro = false;
            
            std::cout << "Corretor " << corretor.id << std::endl;
            
            std::vector<Agendamento> agendamentos = gerarAgendamentoCorretor(imoveis, corretor);
            
            for (const auto& agendamento : agendamentos) {
                std::cout << std::setfill('0') << std::setw(2) << agendamento.hora << ":"
                          << std::setw(2) << agendamento.minuto << " Imóvel " 
                          << agendamento.imovelId << std::endl;
            }
        }
    }
    
    return 0;
} 