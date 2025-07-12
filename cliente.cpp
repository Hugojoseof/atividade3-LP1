#include "cliente.h"

int Cliente::nextId = 1;

Cliente::Cliente(const std::string& telefone, const std::string& nome)
    : id(nextId++), telefone(telefone), nome(nome) {
} 