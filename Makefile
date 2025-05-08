NAME        = raytracing

SRC         = challenge/firstscene.c \
			  src/tuples.c \
			  src/drawing.c \
			  src/matrix.c	\
			  src/ray.c \
			  src/scene.c \
			  src/sphere.c \
			  src/transformation.c \
			  src/render.c

OBJ         = $(SRC:.c=.o)

INC         = -I./inc/

CXX         = gcc
CXXFLAGS    = -Wall -Wextra -Werror

all: $(NAME)

$(NAME): $(OBJ)
	@echo "Linking..."
	@$(CXX) $(CXXFLAGS) -o $@ $^ -lm -lSDL2
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