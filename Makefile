mainfile := "src/*"
INCLUDE1 = "include"
INCLUDE2 = "resource/Shaders"
LIB		= "lib"
LINK = -lglfw3 -lopengl32 -lkernel32 -luser32 -lgdi32 -lwinmm
CC = g++


all:
	$(CC) $(mainfile)	\
		-I$(INCLUDE1)	\
		-I$(INCLUDE2) 	\
		-L$(LIB)		\
		$(LINK)			\
		-o	run.exe
	./run.exe

