/**
 * @file imovel.h
 * @brief Definição da classe Imovel para o sistema de agendamento imobiliário
 * 
 * Esta classe representa um imóvel que será avaliado pelos corretores.
 * Cada imóvel possui características específicas como tipo, localização e preço.
 */

#ifndef IMOVEL_H
#define IMOVEL_H

#include <string>

/**
 * @enum TipoImovel
 * @brief Enumeração dos tipos de imóveis disponíveis no sistema
 */
enum TipoImovel {
    Casa,        ///< Casa residencial
    Apartamento, ///< Apartamento
    Terreno      ///< Terreno
};

/**
 * @class Imovel
 * @brief Representa um imóvel no sistema de agendamento
 * 
 * Um imóvel possui informações sobre seu tipo, localização geográfica,
 * proprietário, endereço e preço. Estas informações são utilizadas
 * para calcular rotas otimizadas e gerar agendamentos.
 */
class Imovel {
private:
    static int nextId; ///< Contador estático para gerar IDs únicos
    
public:
    int id;                    ///< ID único do imóvel (auto-incremento)
    TipoImovel tipo;           ///< Tipo do imóvel (Casa, Apartamento, Terreno)
    int proprietarioId;        ///< ID do cliente proprietário do imóvel
    double latitude;           ///< Latitude da localização do imóvel
    double longitude;          ///< Longitude da localização do imóvel
    std::string endereco;      ///< Endereço completo do imóvel
    double preco;              ///< Preço do imóvel
    
    /**
     * @brief Construtor da classe Imovel
     * @param tipo Tipo do imóvel
     * @param propId ID do proprietário
     * @param lat Latitude da localização
     * @param lon Longitude da localização
     * @param preco Preço do imóvel
     * @param endereco Endereço completo
     */
    Imovel(TipoImovel tipo, int propId, double lat, double lon, double preco, const std::string& endereco);
    
    /**
     * @brief Converte uma string para o tipo de imóvel correspondente
     * @param tipoStr String representando o tipo ("Casa", "Apartamento", "Terreno")
     * @return TipoImovel correspondente à string
     * @note Retorna Casa como padrão se a string não for reconhecida
     */
    static TipoImovel stringToTipo(const std::string& tipoStr);
};

#endif 