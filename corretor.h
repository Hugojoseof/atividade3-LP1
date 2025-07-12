#ifndef CORRETOR_H
#define CORRETOR_H

#include <string>
#include <vector>

class Corretor {
private:
    static int nextId;
    
public:
    int id;
    std::string telefone;
    bool avaliador;
    double latitude;
    double longitude;
    std::string nome;
    std::vector<int> imoveisAtribuidos;
    
    Corretor(const std::string& telefone, bool avaliador, double lat, double lon, const std::string& nome);
    
    bool isAvaliador() const;
    void adicionarImovel(int imovelId);
};

#endif 