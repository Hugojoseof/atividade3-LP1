# Makefile para o Projeto Final - Imobiliária - Agendamento de Avaliações
# 
# Este Makefile fornece comandos para compilar, testar e limpar o projeto.
# 
# Comandos disponíveis:
#   make        - Compila o projeto gerando o executável 'imobiliaria'
#   make test   - Executa todos os testes fornecidos
#   make clean  - Remove arquivos gerados pela compilação
#   make help   - Mostra esta ajuda
#   make debug  - Compila com flags de debug
#   make release - Compila com flags de otimização

# Configurações do compilador
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic
DEBUG_FLAGS = -g -O0 -DDEBUG
RELEASE_FLAGS = -O2 -DNDEBUG

# Arquivos fonte
SRCS = main.cpp corretor.cpp cliente.cpp imovel.cpp

# Nome do executável
TARGET = imobiliaria

# Arquivos de teste
TEST_FILES = teste1_entrada.txt teste2_entrada.txt teste3_entrada.txt
TEST_OUTPUTS = saida1.txt saida2.txt saida3.txt
TEST_EXPECTED = teste1_saida.txt teste2_saida.txt teste3_saida.txt

# Regra principal (compilação padrão)
$(TARGET): $(SRCS)
	@echo "Compilando $(TARGET)..."
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)
	@echo "Compilação concluída com sucesso!"

# Regra para compilação com debug
debug: CXXFLAGS += $(DEBUG_FLAGS)
debug: $(TARGET)
	@echo "Compilação com debug concluída!"

# Regra para compilação com otimização
release: CXXFLAGS += $(RELEASE_FLAGS)
release: $(TARGET)
	@echo "Compilação com otimização concluída!"

# Regra para limpeza
clean:
	@echo "Removendo arquivos gerados..."
	rm -f $(TARGET) $(TEST_OUTPUTS)
	@echo "Limpeza concluída!"

# Regra para testar todos os casos
test: $(TARGET)
	@echo "=== Executando Testes ==="
	@echo "Testando com teste1..."
	./$(TARGET) < $(word 1, $(TEST_FILES)) > $(word 1, $(TEST_OUTPUTS))
	@if diff -q $(word 1, $(TEST_OUTPUTS)) $(word 1, $(TEST_EXPECTED)) > /dev/null; then \
		echo "✓ Teste 1: PASSOU"; \
	else \
		echo "✗ Teste 1: FALHOU"; \
		echo "Diferenças encontradas:"; \
		diff $(word 1, $(TEST_OUTPUTS)) $(word 1, $(TEST_EXPECTED)); \
	fi
	
	@echo "Testando com teste2..."
	./$(TARGET) < $(word 2, $(TEST_FILES)) > $(word 2, $(TEST_OUTPUTS))
	@if diff -q $(word 2, $(TEST_OUTPUTS)) $(word 2, $(TEST_EXPECTED)) > /dev/null; then \
		echo "✓ Teste 2: PASSOU"; \
	else \
		echo "✗ Teste 2: FALHOU"; \
		echo "Diferenças encontradas:"; \
		diff $(word 2, $(TEST_OUTPUTS)) $(word 2, $(TEST_EXPECTED)); \
	fi
	
	@echo "Testando com teste3..."
	./$(TARGET) < $(word 3, $(TEST_FILES)) > $(word 3, $(TEST_OUTPUTS))
	@if diff -q $(word 3, $(TEST_OUTPUTS)) $(word 3, $(TEST_EXPECTED)) > /dev/null; then \
		echo "✓ Teste 3: PASSOU"; \
	else \
		echo "✗ Teste 3: FALHOU"; \
		echo "Diferenças encontradas:"; \
		diff $(word 3, $(TEST_OUTPUTS)) $(word 3, $(TEST_EXPECTED)); \
	fi
	
	@echo "=== Limpando arquivos temporários ==="
	@rm -f $(TEST_OUTPUTS)
	@echo "Todos os testes concluídos!"

# Regra para testar um caso específico
test1: $(TARGET)
	@echo "Executando teste1..."
	./$(TARGET) < teste1_entrada.txt > saida1.txt
	@if diff -q saida1.txt teste1_saida.txt > /dev/null; then \
		echo "✓ Teste 1: PASSOU"; \
	else \
		echo "✗ Teste 1: FALHOU"; \
		diff saida1.txt teste1_saida.txt; \
	fi
	@rm -f saida1.txt

test2: $(TARGET)
	@echo "Executando teste2..."
	./$(TARGET) < teste2_entrada.txt > saida2.txt
	@if diff -q saida2.txt teste2_saida.txt > /dev/null; then \
		echo "✓ Teste 2: PASSOU"; \
	else \
		echo "✗ Teste 2: FALHOU"; \
		diff saida2.txt teste2_saida.txt; \
	fi
	@rm -f saida2.txt

test3: $(TARGET)
	@echo "Executando teste3..."
	./$(TARGET) < teste3_entrada.txt > saida3.txt
	@if diff -q saida3.txt teste3_saida.txt > /dev/null; then \
		echo "✓ Teste 3: PASSOU"; \
	else \
		echo "✗ Teste 3: FALHOU"; \
		diff saida3.txt teste3_saida.txt; \
	fi
	@rm -f saida3.txt

# Regra para mostrar ajuda
help:
	@echo "=== Makefile - Projeto Imobiliária ==="
	@echo ""
	@echo "Comandos disponíveis:"
	@echo "  make        - Compila o projeto (padrão)"
	@echo "  make debug  - Compila com flags de debug"
	@echo "  make release- Compila com flags de otimização"
	@echo "  make test   - Executa todos os testes"
	@echo "  make test1  - Executa apenas o teste 1"
	@echo "  make test2  - Executa apenas o teste 2"
	@echo "  make test3  - Executa apenas o teste 3"
	@echo "  make clean  - Remove arquivos gerados"
	@echo "  make help   - Mostra esta ajuda"
	@echo ""
	@echo "Exemplo de uso:"
	@echo "  ./imobiliaria < entrada.txt > saida.txt"

# Regra para verificar se o executável existe
check: $(TARGET)
	@echo "✓ Executável $(TARGET) encontrado e pronto para uso!"

# Regra para mostrar informações do projeto
info:
	@echo "=== Informações do Projeto ==="
	@echo "Nome: Imobiliária - Agendamento de Avaliações"
	@echo "Compilador: $(CXX)"
	@echo "Padrão C++: C++17"
	@echo "Executável: $(TARGET)"
	@echo "Arquivos fonte: $(SRCS)"
	@echo "Arquivos de teste: $(TEST_FILES)"

# Marca as regras que não geram arquivos
.PHONY: clean test test1 test2 test3 help check info debug release 