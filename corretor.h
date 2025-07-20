/**
 * @file corretor.h
 * @brief Definição da classe Corretor para o sistema de agendamento imobiliário
 * 
 * Esta classe representa um corretor imobiliário que pode ou não ser um avaliador.
 * Corretores avaliadores são responsáveis por realizar as avaliações dos imóveis.
 */

#ifndef CORRETOR_H
#define CORRETOR_H

#include <string>
#include <vector>

/**
 * @class Corretor
 * @brief Representa um corretor imobiliário no sistema
 * 
 * Um corretor possui informações pessoais, localização geográfica e pode
 * ser ou não um avaliador. Corretores avaliadores recebem imóveis para
 * avaliar através do algoritmo de distribuição Round-Robin.
 */
class Corretor {
private:
    static int nextId; ///< Contador estático para gerar IDs únicos
    
public:
    int id;                           ///< ID único do corretor (auto-incremento)
    std::string telefone;             ///< Número de telefone do corretor
    bool avaliador;                   ///< Indica se o corretor é um avaliador
    double latitude;                  ///< Latitude da localização do corretor
    double longitude;                 ///< Longitude da localização do corretor
    std::string nome;                 ///< Nome completo do corretor
    std::vector<int> imoveisAtribuidos; ///< IDs dos imóveis atribuídos a este corretor
    
    /**
     * @brief Construtor da classe Corretor
     * @param telefone Número de telefone do corretor
     * @param avaliador Indica se o corretor é um avaliador
     * @param lat Latitude da localização
     * @param lon Longitude da localização
     * @param nome Nome completo do corretor
     */
    Corretor(const std::string& telefone, bool avaliador, double lat, double lon, const std::string& nome);
    
    /**
     * @brief Verifica se o corretor é um avaliador
     * @return true se for avaliador, false caso contrário
     */
    bool isAvaliador() const;
    
    /**
     * @brief Adiciona um imóvel à lista de imóveis atribuídos ao corretor
     * @param imovelId ID do imóvel a ser adicionado
     */
    void adicionarImovel(int imovelId);
};

#endif 