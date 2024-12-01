SRC = $(shell find src -type f -name "*.cpp")
NAME = R-Type

CXXFLAGS += --std=c++20 -g -Wall -Wextra

SFML_FLAGS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
LUA_FLAGS = -I/usr/include/lua5.4 -llua5.4

FLAGS = $(SFML_FLAGS) $(LUA_FLAGS)


all:
	g++ $(CXXFLAGS) $(SRC) -o $(NAME) $(FLAGS)

debug:
	g++ $(CXXFLAGS) -DDEBUG $(SRC) -o $(NAME) $(FLAGS)

re: clean all


clean:
	rm $(NAME) || true

.Phony: all clean
