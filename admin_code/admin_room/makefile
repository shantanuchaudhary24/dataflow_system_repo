ifndef DEBUG
       DEBUG = 1
endif

ifndef NUM_ROOMS
       NUM_ROOMS = 10
endif

create_res_db.o :
	g++ -c src/create_res_db.cpp -Wall

admin : create_res_db.o
	g++ -o admin create_res_db.o -Wall
	./admin $(NUM_ROOMS) $(DEBUG)

.PHONY:clean
clean:
	rm -f admin create_res_db.o   		  		  		  		  		  		  		  
