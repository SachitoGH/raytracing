NAME        = raytracing

SRC         = clock_test.c \
			  tuples.c \
			  drawing.c \
			  matrix.c

OBJ         = $(SRC:.c=.o)

INC         = 

CXX         = gcc
CXXFLAGS    = -Wall -Wextra -Werror

all: $(NAME)

$(NAME): $(OBJ)
	@echo "Linking..."
	@$(CXX) $(CXXFLAGS) -o $@ $^ -lm
	@echo "Executable created!"

%.o: %.c
	@echo "Compiling $<"
	@$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@
	@echo "Compiled $< OK!"

clean:
	@echo "Cleaning object files..."
	@rm -f $(OBJ)
	@echo "Cleaned!"

fclean: clean
	@echo "Removing executable..."
	@rm -f $(NAME)
	@echo "Removed!"

re: fclean all

.PHONY: all clean fclean re