#include "corretor.h"

int Corretor::nextId = 1;

Corretor::Corretor(const std::string& telefone, bool avaliador, double lat, double lon, const std::string& nome)
    : id(nextId++), telefone(telefone), avaliador(avaliador), latitude(lat), longitude(lon), nome(nome) {
}

bool Corretor::isAvaliador() const {
    return avaliador;
}

void Corretor::adicionarImovel(int imovelId) {
    imoveisAtribuidos.push_back(imovelId);
} 