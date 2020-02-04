TARGET=run
OBJECTFILES=main.o shader.o mesh.o model.o glad.o

CC=gcc
CXX=g++

IFLAGS=-I./includes/ -I./includes/glm
LDFLAGS=-Wl,-rpath,/usr/local/lib -lGL -lglfw3 -lrt -lm -ldl -lX11 -lpthread -lxcb -lXau -lXdmcp -lassimp

$(TARGET) : $(OBJECTFILES)
	$(CXX) -o $@ $^ $(LDFLAGS)
%.o : %.cpp
	$(CXX) -c $(IFLAGS) $^
%.o : %.c
	$(CC) -c $(IFLAGS) $^
clean:
	rm -f *.o
	rm -f *.gch
