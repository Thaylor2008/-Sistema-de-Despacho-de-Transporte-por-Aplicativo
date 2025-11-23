# =============================================================
# Variáveis de Configuração
# =============================================================

# Define o nome do executável final, que DEVE ser 'tp3.out'
EXECUTABLE = tp2.out

# Define as pastas do projeto
BINDIR = bin
OBJDIR = obj
SRCDIR = src
INCDIR = include

# Compilador C++11 (Requisito do TP)
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -Wpedantic -I$(INCDIR) -g 

# Bibliotecas (usado para matemática)
LDLIBS = -lm

# Lista todos os arquivos .cpp na pasta src.
SOURCES = $(SRCDIR)/main.cpp \
	$(SRCDIR)/Corrida.cpp \
	$(SRCDIR)/Demanda.cpp \
	$(SRCDIR)/Parada.cpp \
	$(SRCDIR)/Trecho.cpp \
	$(SRCDIR)/heap.cpp

# Cria a lista de arquivos objeto (.o) correspondentes
OBJECTS = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SOURCES))

# =============================================================
# Metas Principais
# =============================================================

# Meta padrão: Compila tudo e cria o executável
all: directories $(BINDIR)/$(EXECUTABLE)

# Cria o executável final tp3.out no diretório bin
$(BINDIR)/$(EXECUTABLE): $(OBJECTS)
	@echo "--- Ligando objetos para criar o executável: $(EXECUTABLE) ---"
	$(CXX) $(OBJECTS) -o $@ $(LDLIBS)
	@echo "Executável pronto em: $@"

# =============================================================
# Regras de Compilação
# =============================================================

# Regra genérica para compilar arquivos .cpp em arquivos .o
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@echo "Compilando: $< -> $@"
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Regra genérica para compilar arquivos .cpp que dependem de headers
$(OBJECTS): $(INCDIR)/Demanda.hpp $(INCDIR)/Parada.hpp $(INCDIR)/Trecho.hpp $(INCDIR)/Corrida.hpp $(INCDIR)/heap.hpp

# =============================================================
# Metas de Limpeza e Diretórios
# =============================================================

# Cria os diretórios bin e obj se não existirem
directories:
	@mkdir -p $(BINDIR)
	@mkdir -p $(OBJDIR)

# Limpa todos os arquivos objeto e o executável
clean:
	@echo "--- Limpando arquivos objeto e executável ---"
	@rm -rf $(OBJDIR)/* $(BINDIR)/*

# Limpa tudo, incluindo os diretórios criados (útil para submissão)
fclean: clean
	@echo "--- Limpando diretórios bin e obj ---"
	@rm -rf $(OBJDIR) $(BINDIR)

# Meta de teste
test: all
	@echo "--- Executando teste (Substitua 'entrada.txt' pelo nome do seu arquivo de teste) ---"
	@./$(BINDIR)/$(EXECUTABLE) entrada.txt

.PHONY: all clean fclean directories test