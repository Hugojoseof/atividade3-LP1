CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic

# Arquivos fonte
SRCS = main.cpp corretor.cpp cliente.cpp imovel.cpp

# Nome do executável
TARGET = imobiliaria

# Regra principal
$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

# Regra para limpeza
clean:
	rm -f $(TARGET)

# Regra para testar
test: $(TARGET)
	@echo "Testando com teste1..."
	./$(TARGET) < teste1_entrada.txt > saida1.txt
	@if diff -q saida1.txt teste1_saida.txt > /dev/null; then echo "Teste 1: PASSOU"; else echo "Teste 1: FALHOU"; fi
	@echo "Testando com teste2..."
	./$(TARGET) < teste2_entrada.txt > saida2.txt
	@if diff -q saida2.txt teste2_saida.txt > /dev/null; then echo "Teste 2: PASSOU"; else echo "Teste 2: FALHOU"; fi
	@echo "Testando com teste3..."
	./$(TARGET) < teste3_entrada.txt > saida3.txt
	@if diff -q saida3.txt teste3_saida.txt > /dev/null; then echo "Teste 3: PASSOU"; else echo "Teste 3: FALHOU"; fi
	@rm -f saida1.txt saida2.txt saida3.txt

.PHONY: clean test 