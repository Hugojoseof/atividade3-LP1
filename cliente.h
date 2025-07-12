#ifndef CLIENTE_H
#define CLIENTE_H

#include <string>

class Cliente {
private:
    static int nextId;
    
public:
    int id;
    std::string telefone;
    std::string nome;
    
    Cliente(const std::string& telefone, const std::string& nome);
};

#endif 