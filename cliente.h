/**
 * @file cliente.h
 * @brief Definição da classe Cliente para o sistema de agendamento imobiliário
 * 
 * Esta classe representa um cliente que pode ser proprietário de imóveis
 * que serão avaliados pelos corretores.
 */

#ifndef CLIENTE_H
#define CLIENTE_H

#include <string>

/**
 * @class Cliente
 * @brief Representa um cliente no sistema de agendamento
 * 
 * Um cliente possui informações básicas como nome e telefone.
 * Clientes podem ser proprietários de imóveis que serão avaliados
 * pelos corretores avaliadores.
 */
class Cliente {
private:
    static int nextId; ///< Contador estático para gerar IDs únicos
    
public:
    int id;              ///< ID único do cliente (auto-incremento)
    std::string telefone; ///< Número de telefone do cliente
    std::string nome;     ///< Nome completo do cliente
    
    /**
     * @brief Construtor da classe Cliente
     * @param telefone Número de telefone do cliente
     * @param nome Nome completo do cliente
     */
    Cliente(const std::string& telefone, const std::string& nome);
};

#endif 