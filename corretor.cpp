/**
 * @file corretor.cpp
 * @brief Implementação da classe Corretor
 * 
 * Este arquivo contém a implementação dos métodos da classe Corretor,
 * incluindo o construtor e métodos para gerenciar imóveis atribuídos.
 */

#include "corretor.h"

// Inicialização do contador estático de IDs
int Corretor::nextId = 1;

/**
 * @brief Construtor da classe Corretor
 * 
 * Inicializa um novo corretor com as informações fornecidas e
 * atribui automaticamente um ID único sequencial.
 * 
 * @param telefone Número de telefone do corretor
 * @param avaliador Indica se o corretor é um avaliador (true) ou não (false)
 * @param lat Latitude da localização do corretor
 * @param lon Longitude da localização do corretor
 * @param nome Nome completo do corretor
 */
Corretor::Corretor(const std::string& telefone, bool avaliador, double lat, double lon, const std::string& nome)
    : id(nextId++), telefone(telefone), avaliador(avaliador), latitude(lat), longitude(lon), nome(nome) {
}

/**
 * @brief Verifica se o corretor é um avaliador
 * 
 * @return true se o corretor for um avaliador, false caso contrário
 */
bool Corretor::isAvaliador() const {
    return avaliador;
}

/**
 * @brief Adiciona um imóvel à lista de imóveis atribuídos ao corretor
 * 
 * Este método é usado durante a distribuição Round-Robin dos imóveis
 * entre os corretores avaliadores.
 * 
 * @param imovelId ID do imóvel a ser adicionado à lista do corretor
 */
void Corretor::adicionarImovel(int imovelId) {
    imoveisAtribuidos.push_back(imovelId);
} 