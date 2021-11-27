NAME=BinanceBoostTestTask
all: $(NAME).o
	g++ -lboost_system -lboost_thread -lpthread  -lssl -lcrypto -ljsoncpp  $^ streaming_websocket.o -o $(NAME)
$(NAME).o: main.cpp
	g++ -c main.cpp -o $(NAME).o
streaming_websocket.o: streaming_websocket.cpp streaming_websocket.h
	g++ -c streaming_websocket.cpp -o streaming_websocket.o
clean:
	rm $(NAME) $(NAME).o websocket.o
