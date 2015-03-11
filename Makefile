CC   := g++
LIBS := `pkg-config --libs opencv` -ldl -lm 
ELF  := fishIn.a
#OPTIONS = -O2 -g -Wall # -g for debug, -O2 for optimise and -Wall additional messages
INCLUDES = `pkg-config --cflags opencv`-I . # Directory for header file
OBJS = mouse.o # List of objects to be build
.PHONY: all clean # To declare all, clean are not files
all: ${OBJS}
	@echo "Building all" # To print "Building.." message
	${CC} ${OBJS}  -o ${ELF} ${OPTIONS} ${LIBS} ${INCLUDES} 
%.o: %.cpp  # % pattern wildcard matching
	@echo "Building " $@
	${CC} ${OPTIONS}  -c $*.cpp ${INCLUDES} `pkg-config --cflags opencv` 
clean:
	@echo "cleaning Up"
	rm -rf *.o
	-rm ${ELF}



