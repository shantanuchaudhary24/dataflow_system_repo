ifndef ADDRESS
	ADDRESS = localhost
endif	

ifndef PORT
	PORT = 5555
endif	

connection="\#define PORT $(PORT)\n\
	   \#define ADDRESS \"$(ADDRESS)\""

client:
	echo $(connection) > inc/connection_header.h
	clear
	g++ -o client src/ui/*.cpp src/networking/*.cpp src/main/*cpp -lglut -lGLU -lglui -lpthread -Wall
	./client

compile:
	echo $(connection) > inc/connection_header.h
	clear
	g++ -o client src/ui/*.cpp src/networking/*.cpp src/main/*cpp -lglut -lGLU -lglui -lpthread

.PHONY: clean
clean:
	rm -f client
	clear
