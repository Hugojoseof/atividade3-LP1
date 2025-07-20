/**
 * Projeto Final - Imobiliária - Agendamento de Avaliações
 * 
 * Este programa implementa um sistema de agendamento automático de avaliações
 * de imóveis entre corretores-avaliadores, utilizando algoritmos de otimização
 * para minimizar o tempo de deslocamento.
 * 
 * Algoritmo implementado:
 * 1. Distribuição Round-Robin dos imóveis entre avaliadores
 * 2. Para cada avaliador: algoritmo do vizinho mais próximo
 * 3. Cálculo de tempo: 2 min/km deslocamento + 1 hora avaliação
 * 

 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <sstream>
#include <limits>
#include <stdexcept>
#include "corretor.h"
#include "cliente.h"
#include "imovel.h"

constexpr double EARTH_R = 6371.0; // Raio da Terra em km
constexpr int HORA_INICIO = 9;     // Hora de início das avaliações
constexpr int MINUTOS_INICIO = 0;  // Minuto de início
constexpr int DURACAO_AVALIACAO = 60; // Duração da avaliação em minutos
constexpr double TEMPO_DESLOCAMENTO_POR_KM = 2.0; // Minutos por km

/**
 * Calcula a distância em km entre duas coordenadas geográficas
 * usando a fórmula de Haversine
 * 
 * @param lat1 Latitude do primeiro ponto
 * @param lon1 Longitude do primeiro ponto
 * @param lat2 Latitude do segundo ponto
 * @param lon2 Longitude do segundo ponto
 * @return Distância em quilômetros
 */
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

/**
 * Estrutura para armazenar informações de um agendamento
 */
struct Agendamento {
    int hora;
    int minuto;
    int imovelId;
    
    Agendamento(int h, int m, int id) : hora(h), minuto(m), imovelId(id) {}
};

/**
 * Converte minutos desde o início do dia (09:00) para formato hora:minuto
 * 
 * @param minutos Minutos desde 09:00
 * @return Par (hora, minuto) no formato 24h
 */
std::pair<int, int> minutosParaHoraMinuto(int minutos) {
    int totalMinutos = HORA_INICIO * 60 + MINUTOS_INICIO + minutos;
    int hora = totalMinutos / 60;
    int minuto = totalMinutos % 60;
    return {hora, minuto};
}

/**
 * Encontra o imóvel mais próximo não visitado para um corretor
 * 
 * @param imoveis Lista de todos os imóveis
 * @param imoveisCorretor IDs dos imóveis atribuídos ao corretor
 * @param visitados Vetor indicando quais imóveis já foram visitados
 * @param latAtual Latitude atual do corretor
 * @param lonAtual Longitude atual do corretor
 * @return ID do imóvel mais próximo, ou -1 se não houver mais imóveis
 */
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

/**
 * Gera o agendamento otimizado para um corretor usando o algoritmo
 * do vizinho mais próximo
 * 
 * @param imoveis Lista de todos os imóveis
 * @param corretor Corretor para o qual gerar o agendamento
 * @return Lista de agendamentos ordenados por horário
 */
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
        
        if (proximoImovel == -1) {
            throw std::runtime_error("Erro: não foi possível encontrar próximo imóvel");
        }
        
        // Calcular tempo de deslocamento
        double distancia = haversine(latAtual, lonAtual, 
                                   imoveis[proximoImovel - 1].latitude, 
                                   imoveis[proximoImovel - 1].longitude);
        int tempoDeslocamento = static_cast<int>(distancia * TEMPO_DESLOCAMENTO_POR_KM);
        
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
        
        // Adicionar tempo da avaliação
        tempoAtual += DURACAO_AVALIACAO;
    }
    
    return agendamentos;
}

/**
 * Valida se um número está dentro de um intervalo válido
 */
bool validarIntervalo(double valor, double min, double max, const std::string& nome) {
    if (valor < min || valor > max) {
        std::cerr << "Erro: " << nome << " deve estar entre " << min << " e " << max << std::endl;
        return false;
    }
    return true;
}

/**
 * Lê e valida os dados de entrada
 */
void lerDados(std::vector<Corretor>& corretores, 
              std::vector<Cliente>& clientes, 
              std::vector<Imovel>& imoveis) {
    
    // Ler corretores
    int numCorretores;
    if (!(std::cin >> numCorretores) || numCorretores <= 0) {
        throw std::runtime_error("Erro: número de corretores inválido");
    }
    
    for (int i = 0; i < numCorretores; ++i) {
        std::string telefone;
        int avaliador;
        double lat, lon;
        
        if (!(std::cin >> telefone >> avaliador >> lat >> lon)) {
            throw std::runtime_error("Erro: dados do corretor " + std::to_string(i + 1) + " inválidos");
        }
        
        // Validar dados
        if (avaliador != 0 && avaliador != 1) {
            throw std::runtime_error("Erro: avaliador deve ser 0 ou 1");
        }
        if (!validarIntervalo(lat, -90.0, 90.0, "latitude")) {
            throw std::runtime_error("Erro: latitude inválida");
        }
        if (!validarIntervalo(lon, -180.0, 180.0, "longitude")) {
            throw std::runtime_error("Erro: longitude inválida");
        }
        
        std::string nome;
        std::getline(std::cin >> std::ws, nome);
        
        if (nome.empty()) {
            throw std::runtime_error("Erro: nome do corretor não pode estar vazio");
        }
        
        corretores.emplace_back(telefone, avaliador == 1, lat, lon, nome);
    }
    
    // Ler clientes
    int numClientes;
    if (!(std::cin >> numClientes) || numClientes < 0) {
        throw std::runtime_error("Erro: número de clientes inválido");
    }
    
    for (int i = 0; i < numClientes; ++i) {
        std::string telefone;
        if (!(std::cin >> telefone)) {
            throw std::runtime_error("Erro: telefone do cliente " + std::to_string(i + 1) + " inválido");
        }
        
        std::string nome;
        std::getline(std::cin >> std::ws, nome);
        
        if (nome.empty()) {
            throw std::runtime_error("Erro: nome do cliente não pode estar vazio");
        }
        
        clientes.emplace_back(telefone, nome);
    }
    
    // Ler imóveis
    int numImoveis;
    if (!(std::cin >> numImoveis) || numImoveis <= 0) {
        throw std::runtime_error("Erro: número de imóveis inválido");
    }
    
    for (int i = 0; i < numImoveis; ++i) {
        std::string tipo;
        int propId;
        double lat, lon, preco;
        
        if (!(std::cin >> tipo >> propId >> lat >> lon >> preco)) {
            throw std::runtime_error("Erro: dados do imóvel " + std::to_string(i + 1) + " inválidos");
        }
        
        // Validar dados
        if (propId <= 0 || propId > static_cast<int>(clientes.size())) {
            throw std::runtime_error("Erro: ID do proprietário inválido");
        }
        if (!validarIntervalo(lat, -90.0, 90.0, "latitude")) {
            throw std::runtime_error("Erro: latitude inválida");
        }
        if (!validarIntervalo(lon, -180.0, 180.0, "longitude")) {
            throw std::runtime_error("Erro: longitude inválida");
        }
        if (preco <= 0) {
            throw std::runtime_error("Erro: preço deve ser positivo");
        }
        
        std::string endereco;
        std::getline(std::cin >> std::ws, endereco);
        
        if (endereco.empty()) {
            throw std::runtime_error("Erro: endereço não pode estar vazio");
        }
        
        imoveis.emplace_back(Imovel::stringToTipo(tipo), propId, lat, lon, preco, endereco);
    }
}

/**
 * Função principal do programa
 */
int main() {
    try {
        std::vector<Corretor> corretores;
        std::vector<Cliente> clientes;
        std::vector<Imovel> imoveis;
        
        // Ler e validar dados de entrada
        lerDados(corretores, clientes, imoveis);
        
        // Filtrar apenas corretores avaliadores
        std::vector<Corretor*> avaliadores;
        for (auto& corretor : corretores) {
            if (corretor.isAvaliador()) {
                avaliadores.push_back(&corretor);
            }
        }
        
        // Verificar se há avaliadores
        if (avaliadores.empty()) {
            throw std::runtime_error("Erro: não há corretores avaliadores");
        }
        
        // Verificar se há imóveis para avaliar
        if (imoveis.empty()) {
            throw std::runtime_error("Erro: não há imóveis para avaliar");
        }
        
        // Distribuir imóveis (Round-Robin)
        for (int i = 0; i < static_cast<int>(imoveis.size()); ++i) {
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
        
    } catch (const std::exception& e) {
        std::cerr << "Erro: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Erro desconhecido ocorreu" << std::endl;
        return 1;
    }
    
    return 0;
} 