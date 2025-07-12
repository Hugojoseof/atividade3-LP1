#ifndef IMOVEL_H
#define IMOVEL_H

#include <string>

enum TipoImovel {
    Casa,
    Apartamento,
    Terreno
};

class Imovel {
private:
    static int nextId;
    
public:
    int id;
    TipoImovel tipo;
    int proprietarioId;
    double latitude;
    double longitude;
    std::string endereco;
    double preco;
    
    Imovel(TipoImovel tipo, int propId, double lat, double lon, double preco, const std::string& endereco);
    
    static TipoImovel stringToTipo(const std::string& tipoStr);
};

#endif 