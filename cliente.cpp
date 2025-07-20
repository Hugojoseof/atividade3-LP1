/**
 * @file cliente.cpp
 * @brief Implementação da classe Cliente
 * 
 * Este arquivo contém a implementação dos métodos da classe Cliente,
 * incluindo o construtor.
 */

#include "cliente.h"

// Inicialização do contador estático de IDs
int Cliente::nextId = 1;

/**
 * @brief Construtor da classe Cliente
 * 
 * Inicializa um novo cliente com as informações fornecidas e
 * atribui automaticamente um ID único sequencial.
 * 
 * @param telefone Número de telefone do cliente
 * @param nome Nome completo do cliente
 */
Cliente::Cliente(const std::string& telefone, const std::string& nome)
    : id(nextId++), telefone(telefone), nome(nome) {
} 