.PHONY all clean

vpath %.h ./inc
vpath %.c ./src
vpath %.o ./obj

FINAL_TARGET=/root/biometria_kinematika
LIBS=-lbcm2835 -lncurses -lpthread -lm
INCLUDES=-I./inc
OBJECTS=main.o udp_send-thread.o 

all:$(FINAL_TARGET)

$(FINAL_TARGET): $(OBJECTS)
	gcc $^ -o $(FINAL_TARGET) $(LIBS) -g -Wall

main.o: main.c main.h 
	gcc $< -c -o obj/main.o $(INCLUDES) $(LIBS) -g -Wall -O0
udp_send_thread.o: udp_send_thread.c udp_send_thread.h 
	gcc $< -c -o obj/udp_send_thread.o $(INCLUDES) $(LIBS) -g -Wall -O0

clean:
	rm -rf $(OBJECTS) $(FINAL_TARGET)
