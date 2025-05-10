NAME        = raytracing

SRC         = src/main.c \
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

ifeq ($(shell uname), Linux)
	MLX_DIR		=	minilibx
	MLX_O_FLAGS	=	-L $(MLX_DIR) -lmlx -lXext -lX11 -lm -lz
	MLX_LIB		=	$(MLX_DIR)/libmlx_Linux.a
else
	MLX_DIR		=	minilibx_macos
	MLX_O_FLAGS	=	-L $(MLX_DIR) -lmlx -framework OpenGL -framework AppKit
	MLX_LIB		=	$(MLX_DIR)/libmlx.a
endif

MLX_C_FLAGS =	-I $(MLX_DIR)

all: $(NAME)

$(NAME): $(MLX_LIB) $(OBJ)
	@echo "Linking..."
	@$(CXX) $(CXXFLAGS) $(OBJ) $(MLX_O_FLAGS) -o $(NAME) -lm -lSDL2
	@echo "Executable created!"

%.o: %.c
	@echo "Compiling $<"
	@$(CXX) $(CXXFLAGS) $(MLX_C_FLAGS) $(INC) -c $< -o $@
	@echo "Compiled $< OK!"

$(MLX_LIB):	check
			@$(MAKE) -C $(MLX_DIR)

check:

clean:
	@echo "Cleaning object files..."
	@rm -f $(OBJ)
	@$(MAKE) clean -C $(MLX_DIR)
	@echo "Cleaned!"

fclean: clean
	@echo "Removing executable..."
	@rm -f $(NAME)
	@echo "Removed!"

re: fclean all

.PHONY: all clean fclean re check