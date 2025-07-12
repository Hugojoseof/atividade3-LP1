#include "imovel.h"

int Imovel::nextId = 1;

Imovel::Imovel(TipoImovel tipo, int propId, double lat, double lon, double preco, const std::string& endereco)
    : id(nextId++), tipo(tipo), proprietarioId(propId), latitude(lat), longitude(lon), endereco(endereco), preco(preco) {
}

TipoImovel Imovel::stringToTipo(const std::string& tipoStr) {
    if (tipoStr == "Casa") {
        return Casa;
    } else if (tipoStr == "Apartamento") {
        return Apartamento;
    } else if (tipoStr == "Terreno") {
        return Terreno;
    }
    return Casa; // default
} 