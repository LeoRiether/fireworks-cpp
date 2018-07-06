#OBJS specifies which files to compile as part of the project
OBJS = main.cpp

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = out.exe

CPPV = --std=c++0x

#This is the target that compiles our executable
all : $(OBJS)
	g++ $(OBJS) -ID:\SDL2-2.0.8\x86_64-w64-mingw32\include\SDL2 -LD:\SDL2-2.0.8\x86_64-w64-mingw32\lib -w -Wl,-subsystem,windows -lmingw32 -lSDL2main -lSDL2 -o $(OBJ_NAME) $(CPPV)