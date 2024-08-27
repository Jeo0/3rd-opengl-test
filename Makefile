mainfile := "src/*"
INCLUDE = "include"
LIB		= "lib"
LINK = -lglfw3 -lopengl32 -lkernel32 -luser32 -lgdi32 -lwinmm
CC = g++


all:
	$(CC) $(mainfile)	\
		-I$(INCLUDE)	\
		-L$(LIB)		\
		$(LINK)			\
		-o	run.exe
	./run.exe

