#Compiler flags
CC = gcc
CFLAGS = -Wall -ansi -pedantic -fno-common

#Project folders
INCLUDE_PATH = include
LIB_PATH = lib
BUILD_OUTPUT = build\Espadon.lib

SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)

LIBRARIES = $(LIB_PATH)\$(PERCEMAILLE_LIB) $(LIB_PATH)\$(GLFW_LIB)
HEADERS = $(INCLUDE_PATH)\$(PERCEMAILLE_HEADER) $(INCLUDE_PATH)\$(GLFW_HEADER)

#External libraries
LINKER = -lglfw3 -lglew32s -lopengl32 -lpercemaille -lGdi32

PERCEMAILLE_LIB = percemaille.lib
PERCEMAILLE_HEADER = Percemaille.h

GLFW_LIB = libglfw3.a
GLFW_HEADER = GLFW\glfw3.h



#Target rules

all : main

main : $(OBJ)
	ar -rcs $(BUILD_OUTPUT) $(OBJ) build/Percemaille/*.o build/GLFW/*.obj
	ranlib $(BUILD_OUTPUT)

extract_libs :
	cd .\build\Percemaille && ar -x ..\..\$(LIB_PATH)\$(PERCEMAILLE_LIB)
	cd .\build\GLFW && ar -x ..\..\$(LIB_PATH)\$(GLFW_LIB)

%.o : %.c
	$(CC) $(CFLAGS) -IC:$(INCLUDE_PATH) -o $@ -LC:$(LIB_PATH) $(LINKER) -c $<

clean : 
	del .\src\*.o