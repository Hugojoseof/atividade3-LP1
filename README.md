# Projeto Final — Imobiliária - Agendamento de Avaliações

Este trabalho fecha o semestre de LP1. O objetivo deste projeto é implementar um gerador de agendas que distribua as avaliações de imóveis entre corretores-avaliadores, calculando o horário de início de cada visita conforme um algoritmo específico. Seu programa deverá processar dados de entrada sobre corretores, clientes e imóveis, e produzir uma agenda automática de avaliações otimizada.

Todo o código deve compilar em qualquer g++ 11 ou superior (gcc/g++).

---

## Como o programa funciona

Seu programa deve:
1. Ler dados da **entrada padrão** (não de arquivos com `fopen` ou `ifstream`).
2. Escrever resultados na **saída padrão** (usando `cout`).
3. Ser executado **redirecionando um arquivo** para a entrada.

#### Exemplo de execução
```bash
./imobiliaria < entrada.txt
```
Isso significa que o conteúdo do arquivo `entrada.txt` será "enviado" para o seu programa como se fosse digitado no teclado.

#### Como executar com redirecionamento (todas as plataformas)

*   **Windows (Command Prompt ou PowerShell):** `imobiliaria.exe < entrada.txt`
*   **Linux/macOS (Terminal):** `./imobiliaria < entrada.txt`

Seu programa usará `cin` normalmente, mas os dados virão do arquivo. A saída (`cout`) aparecerá no terminal, ou pode ser redirecionada para um arquivo de saída com `>`.

---

## Regras de modelagem

| Entidade     | Campos obrigatórios                                                                                                               |
| ------------ | --------------------------------------------------------------------------------------------------------------------------------- |
| Corretor     | `id` (auto-incremento), `nome`, `telefone`, `bool avaliador`, `lat`, `lng`                                                        |
| Cliente      | `id`, `nome`, `telefone`                                                                                                          |
| Imovel       | `id`, `tipo` (enum `{Casa, Apartamento, Terreno}`), `proprietarioId`, `lat`, `lng`, `endereco` (string livre), `preco` (double)   |

*   Use `static int nextId` ou equivalente para gerar IDs sequenciais para cada entidade, começando em 1.
*   Armazene as coleções de objetos em `std::vector`.

---

## 2 · Formato de Entrada e Saída

### Formato da Entrada Padrão

Seu programa deve ler da entrada padrão **exatamente** neste formato:
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
**Observações:**
- **Texto livre sempre no fim da linha.** Assim você pode ler todos os valores numéricos com `>>` e depois usar `std::getline` para pegar o restante da linha.
- `avaliador`: `1` para sim, `0` para não.
- `nome` e `endereco` podem conter espaços.

#### Exemplo de leitura de uma linha
```cpp
std::string linha;
// ler telefone, avaliador, lat, lon
std::string telefone; int avaliador; double lat, lon;
std::cin >> telefone >> avaliador >> lat >> lon;
std::string nome;
std::getline(std::cin >> std::ws, nome); // lê o restante da linha como nome completo
```
Para o imóvel:
```cpp
std::string tipo; int propId; double lat, lon, preco;
std::cin >> tipo >> propId >> lat >> lon >> preco;
std::string endereco;
std::getline(std::cin >> std::ws, endereco);
```
Use `>> std::ws` para descartar espaços em branco pendentes antes de `getline`.

### Formato da Saída Padrão
Seu programa deve imprimir **apenas** o agendamento, sem nenhum texto adicional.

```
Corretor [ID]
[HH:MM] Imóvel [ID]
[HH:MM] Imóvel [ID]

Corretor [ID]
[HH:MM] Imóvel [ID]
...
```

Exemplo concreto (para 2 corretores e 3 imóveis):

```
Corretor 1
09:04 Imóvel 1
10:11 Imóvel 3

Corretor 2
09:09 Imóvel 2
```

**Observações:**
- Use formato de hora `HH:MM` (com zero à esquerda, se necessário).
- Deixe uma linha em branco entre os agendamentos de corretores diferentes.

---

## 3 · Algoritmo de Agendamento

A avaliação de cada imóvel dura **1 hora**. O tempo de deslocamento é fixo em **2 minutos por quilômetro** em linha reta.

#### Cálculo da distância
Use a função Haversine abaixo para obter a distância em km entre duas coordenadas:
```cpp
#include <cmath>
constexpr double EARTH_R = 6371.0; // Raio da Terra em km

double haversine(double lat1, double lon1, double lat2, double lon2) {
    auto deg2rad = [](double d){ return d * M_PI / 180.0; };
    double dlat = deg2rad(lat2 - lat1);
    double dlon = deg2rad(lon2 - lon1);
    double a = std::pow(std::sin(dlat/2), 2) +
               std::cos(deg2rad(lat1)) * std::cos(deg2rad(lat2)) *
               std::pow(std::sin(dlon/2), 2);
    double c = 2 * std::atan2(std::sqrt(a), std::sqrt(1 - a));
    return EARTH_R * c;
}
```

#### Passo 1: Distribuir Imóveis (Round-Robin)
1. Ordene a lista de imóveis por ID.
2. Atribua os imóveis aos corretores **avaliadores** em sequência (imóvel 1 ao avaliador 1, imóvel 2 ao avaliador 2, etc.). Ao chegar ao último avaliador, volte ao primeiro.

#### Passo 2: Ordenar a Rota (Vizinho Mais Próximo)
Para cada avaliador, partindo de sua localização inicial às **09:00**:
1. Encontre o imóvel **ainda não visitado** mais próximo do ponto atual (localização do corretor ou do último imóvel visitado).
2. Calcule o tempo de deslocamento (`distância × 2 min`) e adicione ao relógio.
3. Agende a visita para o horário calculado e adicione **60 minutos** (duração da avaliação) ao relógio para definir o ponto de partida para o próximo cálculo.
4. Repita até que todos os imóveis daquele corretor sejam agendados.

---

## 4 · Entrega e Avaliação

*   **Prazo:** Siga a data registrada no SIGAA.
*   **Grupo:** Até 3 alunos.
*   **Repositório:** Deve ser **público** no GitHub ou GitLab.

### Requisitos Obrigatórios
1.  **Makefile:** Seu projeto deve ser compilável com `make` e gerar um executável chamado `imobiliaria`.
2.  **Código Modular:** Organize seu código em múltiplos arquivos (`.cpp` e `.h`), como `corretor.h`, `imovel.h`, etc.
3.  **Compatibilidade:** Deve compilar com g++ 11 ou superior.

### Como Compilar e Testar

#### Arquivos Fornecidos
- `teste1_entrada.txt` / `teste1_saida.txt`: Teste básico.
- `teste2_entrada.txt` / `teste2_saida.txt`: Teste mediano.
- `teste3_entrada.txt` / `teste3_saida.txt`: Teste mais complexo.

#### Como Testar sua Solução

```bash
# 1. Compile o programa
make

# 2. Execute com um arquivo de entrada e compare com o gabarito
./imobiliaria < teste1_entrada.txt > minha_saida1.txt
diff minha_saida1.txt teste1_saida.txt
```
Se o comando `diff` não mostrar nada, sua saída está correta. Repita para cada par entrada/saída.

### Processo de Correção
A correção será feita **exatamente** assim:
```bash
git clone https://repositorio_do_aluno
cd repositorio_do_aluno
make
./imobiliaria < entrada_do_professor.txt
```
A saída do seu programa será comparada com a saída esperada.

### Pontos Críticos (Para Não Zerar)
1.  **Repositório acessível publicamente**
2.  **`make` não pode falhar**
3.  **Executável deve se chamar `imobiliaria`**
4.  **Formato de entrada/saída precisamente correto**
5.  **Teste com **todos** os arquivos de entrada/saída fornecidos antes de entregar.

---

## 5 · Dicas de Implementação

#### Makefile Exemplo
```makefile
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -pedantic

# Adicione todos os seus arquivos .cpp aqui
SRCS = main.cpp corretor.cpp imovel.cpp cliente.cpp

imobiliaria: $(SRCS)
	$(CXX) $(CXXFLAGS) -o imobiliaria $(SRCS)

clean:
	rm -f imobiliaria
```

#### Estrutura de Leitura em `main.cpp`
```cpp
#include <iostream>
#include <vector>
#include "corretor.h" // etc.

int main() {
    // Ler corretores
    int numCorretores;
    std::cin >> numCorretores;
    for (int i = 0; i < numCorretores; ++i) {
        // ... use std::cin para ler os dados ...
    }
    
    // Ler clientes e imóveis...
    
    // Chamar a função que gera e imprime o agendamento
    
    return 0;
}
```

#### Formato de Saída com `cout`
```cpp
#include <iomanip>

// Dentro do seu loop de impressão:
std::cout << "Corretor " << corretor.id << std::endl;
std::cout << std::setfill('0') << std::setw(2) << hora << ":" 
          << std::setw(2) << minuto << " Imóvel " << imovel.id << std::endl;
```

---

### Resumo para Não Esquecer
1.  Repositório público no GitHub/GitLab.
2.  `Makefile` funcional que gera o executável `imobiliaria`.
3.  Leitura da entrada padrão (`cin`).
4.  Escrita na saída padrão (`cout`).
5.  Formato de saída **exato**, sem texto extra.
6.  Teste com **todos** os arquivos de entrada/saída fornecidos antes de entregar.

Boa sorte! 