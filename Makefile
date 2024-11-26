SRC = $(wildcard src/*.cpp)
NAME = bs
CXXFLAGS += --std=c++20 -g -Wall -Wextra


all:
	g++ $(CXXFLAGS) $(SRC) -o $(NAME)

debug:
	g++ $(CXXFLAGS) -DDEBUG $(SRC) -o $(NAME)

clean:
	rm $(NAME) || true

.Phony: all clean
