# Documentação Técnica - Sistema de Agendamento Imobiliário

## Visão Geral

Este projeto implementa um sistema de agendamento automático de avaliações de imóveis entre corretores-avaliadores. O sistema utiliza algoritmos de otimização para minimizar o tempo de deslocamento e distribuir eficientemente as avaliações.

## Arquitetura do Sistema

### Estrutura de Classes

#### 1. Classe `Corretor`
- **Responsabilidade**: Representa um corretor imobiliário
- **Atributos principais**:
  - `id`: Identificador único (auto-incremento)
  - `nome`, `telefone`: Informações pessoais
  - `avaliador`: Indica se o corretor é um avaliador
  - `latitude`, `longitude`: Localização geográfica
  - `imoveisAtribuidos`: Lista de IDs dos imóveis atribuídos

#### 2. Classe `Cliente`
- **Responsabilidade**: Representa um cliente/proprietário
- **Atributos principais**:
  - `id`: Identificador único (auto-incremento)
  - `nome`, `telefone`: Informações pessoais

#### 3. Classe `Imovel`
- **Responsabilidade**: Representa um imóvel a ser avaliado
- **Atributos principais**:
  - `id`: Identificador único (auto-incremento)
  - `tipo`: Tipo do imóvel (Casa, Apartamento, Terreno)
  - `proprietarioId`: ID do cliente proprietário
  - `latitude`, `longitude`: Localização geográfica
  - `endereco`: Endereço completo
  - `preco`: Preço do imóvel

## Algoritmos Implementados

### 1. Distribuição Round-Robin
```cpp
// Distribuição sequencial de imóveis entre avaliadores
for (int i = 0; i < numImoveis; ++i) {
    int avaliadorIndex = i % avaliadores.size();
    avaliadores[avaliadorIndex]->adicionarImovel(imoveis[i].id);
}
```

### 2. Algoritmo do Vizinho Mais Próximo
Para cada corretor avaliador:
1. Inicia na localização do corretor às 09:00
2. Encontra o imóvel não visitado mais próximo
3. Calcula tempo de deslocamento (2 min/km)
4. Agenda a visita e adiciona 60 min de avaliação
5. Repete até visitar todos os imóveis atribuídos

### 3. Cálculo de Distância (Fórmula de Haversine)
```cpp
double haversine(double lat1, double lon1, double lat2, double lon2) {
    // Implementação da fórmula de Haversine para coordenadas geográficas
    // Retorna distância em quilômetros
}
```

## Tratamento de Erros

O sistema implementa validação robusta de entrada:

### Validações Implementadas
- **Números de entidades**: Devem ser positivos
- **Coordenadas geográficas**: Latitude (-90 a 90), Longitude (-180 a 180)
- **Campos obrigatórios**: Nomes e endereços não podem estar vazios
- **IDs de proprietário**: Devem referenciar clientes existentes
- **Preços**: Devem ser positivos
- **Flags de avaliador**: Devem ser 0 ou 1

### Tratamento de Exceções
```cpp
try {
    // Código principal
} catch (const std::exception& e) {
    std::cerr << "Erro: " << e.what() << std::endl;
    return 1;
} catch (...) {
    std::cerr << "Erro desconhecido ocorreu" << std::endl;
    return 1;
}
```

## Formato de Entrada

```
[número de corretores]
[telefone] [avaliador] [latitude] [longitude] [nome]
...

[número de clientes]
[telefone] [nome]
...

[número de imóveis]
[tipo] [proprietarioId] [latitude] [longitude] [preco] [endereco]
...
```

### Exemplo de Entrada
```
2
123456789 1 -3.74 -38.52 João Silva
987654321 1 -3.75 -38.55 Maria Santos

2
111222333 Carlos Oliveira
444555666 Ana Costa

3
Casa 1 -3.75 -38.50 300000.0 Rua A, 123
Apartamento 2 -3.77 -38.47 200000.0 Rua B, 456
Terreno 1 -3.76 -38.53 150000.0 Rua C, 789
```

## Formato de Saída

```
Corretor [ID]
[HH:MM] Imóvel [ID]
[HH:MM] Imóvel [ID]

Corretor [ID]
[HH:MM] Imóvel [ID]
...
```

### Exemplo de Saída
```
Corretor 1
09:04 Imóvel 1
10:11 Imóvel 3

Corretor 2
09:18 Imóvel 2
```

## Constantes do Sistema

```cpp
constexpr double EARTH_R = 6371.0;           // Raio da Terra em km
constexpr int HORA_INICIO = 9;               // Hora de início das avaliações
constexpr int MINUTOS_INICIO = 0;            // Minuto de início
constexpr int DURACAO_AVALIACAO = 60;        // Duração da avaliação em minutos
constexpr double TEMPO_DESLOCAMENTO_POR_KM = 2.0; // Minutos por km
```

## Compilação e Execução

### Compilação
```bash
make                    # Compilação padrão
make debug             # Compilação com debug
make release           # Compilação com otimização
```

### Execução
```bash
./imobiliaria < entrada.txt > saida.txt
```

### Testes
```bash
make test              # Executa todos os testes
make test1             # Executa apenas teste 1
make test2             # Executa apenas teste 2
make test3             # Executa apenas teste 3
```

## Análise de Complexidade

### Complexidade Temporal
- **Leitura de dados**: O(n + m + p) onde n, m, p são números de corretores, clientes e imóveis
- **Distribuição Round-Robin**: O(p)
- **Algoritmo do vizinho mais próximo**: O(k²) por corretor, onde k é o número de imóveis por corretor
- **Total**: O(n + m + p + k²)

### Complexidade Espacial
- **Armazenamento de dados**: O(n + m + p)
- **Matriz de distâncias**: O(k²) por corretor
- **Total**: O(n + m + p + k²)

## Limitações e Melhorias Futuras

### Limitações Atuais
1. **Algoritmo guloso**: O vizinho mais próximo não garante a solução ótima global
2. **Tempo fixo**: Não considera variações de tráfego
3. **Sem restrições**: Não considera horários de preferência dos clientes

### Possíveis Melhorias
1. **Algoritmos metaheurísticos**: Implementar simulated annealing ou genetic algorithm
2. **Restrições temporais**: Adicionar janelas de tempo para visitas
3. **Interface gráfica**: Desenvolver interface para visualização das rotas
4. **Persistência**: Adicionar banco de dados para armazenar histórico
5. **API REST**: Criar interface web para o sistema

## Conclusão

O sistema implementa com sucesso os requisitos especificados, fornecendo uma solução robusta e bem documentada para o problema de agendamento de avaliações imobiliárias. A arquitetura modular permite fácil manutenção e extensão futura. 