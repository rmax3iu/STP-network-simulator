# On définit le compilateur à utiliser
CC = gcc

# On active les warnings et le mode débogage
CFLAGS = -Wall -Wextra -g

# On définit le dossier des fichiers source
SRC_DIR = src

# On définit le dossier des fichiers compilés .o
BUILD_DIR = build

# On définit le dossier de l'exécutable final
BIN_DIR = bin

# On définit le chemin complet de l'exécutable final
TARGET = $(BIN_DIR)/sae23

# On récupère automatiquement tous les fichiers .c dans src/
SRCS = $(wildcard $(SRC_DIR)/*.c)

# On transforme chaque src/fichier.c en build/fichier.o
OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

# On définit la règle par défaut : produire l'exécutable final
all : $(TARGET)

# On lie tous les .o ensemble pour produire l'exécutable
# $^ correspond à tous les .o et $@ correspond à l'exécutable cible
$(TARGET) : $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

# On compile chaque .c en .o
# $< correspond au fichier .c source, $@ correspond au fichier .o à produire
$(BUILD_DIR)/%.o : $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# On supprime tous les fichiers compilés .o et l'exécutable
clean :
	rm -f $(BUILD_DIR)/*.o $(TARGET)

# On recompile tout depuis zéro : clean puis all
re : clean all

# On indique que clean et re ne sont pas des fichiers
.PHONY : all clean re