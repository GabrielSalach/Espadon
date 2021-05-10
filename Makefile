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

PERCEMAILLE_SOURCE_PATH = vendors\Percemaille
PERCEMAILLE_LIB = percemaille.lib
PERCEMAILLE_HEADER = Percemaille.h

GLFW_SOURCE_PATH = vendors\glfw\build
GLFW_LIB = libglfw3.a
GLFW_HEADER = GLFW\glfw3.h



#Target rules

all : $(LIBRARIES) $(HEADERS) main

#Percemaille
$(LIB_PATH)\$(PERCEMAILLE_LIB) : 
	@$(MAKE) -C $(PERCEMAILLE_SOURCE_PATH)
	@copy $(PERCEMAILLE_SOURCE_PATH)\$(PERCEMAILLE_LIB) $@

$(INCLUDE_PATH)\$(PERCEMAILLE_HEADER) : 
	@copy $(PERCEMAILLE_SOURCE_PATH)\Percemaille.h $(INCLUDE_PATH)\$(PERCEMAILLE_HEADER)


#GLFW
$(LIB_PATH)\$(GLFW_LIB) :
	@echo BUILDING GLFW...
	@$(MAKE) -C $(GLFW_SOURCE_PATH)
	@copy $(GLFW_SOURCE_PATH)\src\$(GLFW_LIB) $@

$(INCLUDE_PATH)\$(GLFW_HEADER) :
	@xcopy /E /I $(GLFW_SOURCE_PATH)\..\include\GLFW $(INCLUDE_PATH)\GLFW

main : $(OBJ)
	cd .\build\Percemaille && ar -x ..\..\$(LIB_PATH)\$(PERCEMAILLE_LIB)
	cd .\build\GLFW && ar -x ..\..\$(LIB_PATH)\$(GLFW_LIB)

	ar -rcs $(BUILD_OUTPUT) $(OBJ) build/Percemaille/*.o build/GLFW/*.obj
	ranlib $(BUILD_OUTPUT)

%.o : %.c
	$(CC) $(CFLAGS) -IC:$(INCLUDE_PATH) -o $@ -LC:$(LIB_PATH) $(LINKER) -c $<

clean : 
	del .\src\*.o