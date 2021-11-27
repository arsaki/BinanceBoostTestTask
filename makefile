NAME=BinanceBoostTestTask
all: $(NAME).o
	g++ -lboost_system -lboost_thread -lpthread  -lssl -lcrypto  $^ -o $(NAME)
$(NAME).o: main.cpp
	g++ -c main.cpp -o $(NAME).o
clean:
	rm $(NAME) $(NAME).o
