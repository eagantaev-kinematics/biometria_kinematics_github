.PHONY: all clean


vpath %.h ./inc
vpath %.c ./src
vpath %.o ./obj

FINAL_TARGET=biometria_kinematika
LIBS=-lbcm2835 -lncurses -lpthread -lm
INCLUDES=-I./inc
OBJECTS=main.o udp_send_thread.o bcm2835_spi_obj.o 

all:$(FINAL_TARGET)

$(FINAL_TARGET): $(OBJECTS)
	sync
	sync
	sleep 2
	gcc $^ -o $(FINAL_TARGET) $(LIBS) -g -Wall

main.o: main.c main.h 
	gcc $< -c -o obj/main.o $(INCLUDES) $(LIBS) -g -Wall -O0
udp_send_thread.o: udp_send_thread.c udp_send_thread.h 
	gcc $< -c -o obj/udp_send_thread.o $(INCLUDES) $(LIBS) -g -Wall -O0
bcm2835_spi_obj.o:bcm2835_spi_obj.c bcm2835_spi_obj.h
	gcc $< -c -o obj/bcm2835_spi_obj.o $(INCLUDES) $(LIBS) -g -Wall -O0

clean:$(OBJECTS) $(FINAL_TARGET)
	rm -rf $^ 
