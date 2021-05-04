NAME = BattleShips
SRC = main.cpp \
	database.cpp \
	fill.cpp \
	init.cpp \
	move.cpp \
	parameters.cpp \
	print.cpp \
	ai.cpp \

OBJDIR = objects
OBJ := $(SRC:%.cpp=$(OBJDIR)/%.o)
# OBJ = $(SRC:.cpp=.o)
COMPILER = clang++
FLAGS =  -std=c++2a -g  #-Wall -Wextra -Werror
HEADER = battleships.hpp

$(OBJDIR)/%.o: %.cpp $(HEADER)
	$(COMPILER) $(FLAGS) -c $< -o $@

all: $(NAME)

$(NAME): $(OBJ)
	$(COMPILER) $(FLAGS) -o $(NAME) $(OBJ) -lsqlite3

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
