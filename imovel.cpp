/**
 * @file imovel.cpp
 * @brief Implementação da classe Imovel
 * 
 * Este arquivo contém a implementação dos métodos da classe Imovel,
 * incluindo o construtor e métodos para conversão de tipos.
 */

#include "imovel.h"

// Inicialização do contador estático de IDs
int Imovel::nextId = 1;

/**
 * @brief Construtor da classe Imovel
 * 
 * Inicializa um novo imóvel com as informações fornecidas e
 * atribui automaticamente um ID único sequencial.
 * 
 * @param tipo Tipo do imóvel (Casa, Apartamento, Terreno)
 * @param propId ID do cliente proprietário do imóvel
 * @param lat Latitude da localização do imóvel
 * @param lon Longitude da localização do imóvel
 * @param preco Preço do imóvel
 * @param endereco Endereço completo do imóvel
 */
Imovel::Imovel(TipoImovel tipo, int propId, double lat, double lon, double preco, const std::string& endereco)
    : id(nextId++), tipo(tipo), proprietarioId(propId), latitude(lat), longitude(lon), endereco(endereco), preco(preco) {
}

/**
 * @brief Converte uma string para o tipo de imóvel correspondente
 * 
 * Esta função estática converte strings para os tipos de imóvel
 * definidos na enumeração TipoImovel. É utilizada durante a
 * leitura dos dados de entrada.
 * 
 * @param tipoStr String representando o tipo do imóvel
 * @return TipoImovel correspondente à string fornecida
 * @note Se a string não for reconhecida, retorna Casa como padrão
 */
TipoImovel Imovel::stringToTipo(const std::string& tipoStr) {
    if (tipoStr == "Casa") {
        return Casa;
    } else if (tipoStr == "Apartamento") {
        return Apartamento;
    } else if (tipoStr == "Terreno") {
        return Terreno;
    }
    return Casa; // valor padrão caso a string não seja reconhecida
} 