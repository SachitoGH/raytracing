NAME        = raytracing

SRC         = challenge/simple.c \
			  challenge/firstscene.c \
			  challenge/test.c \
			  src/tuples.c \
			  src/drawing.c \
			  src/matrix.c	\
			  src/ray.c \
			  src/scene.c \
			  src/sphere.c \
			  src/transformation.c \
			  src/render.c \
			  src/plane.c \
			  src/cube.c \
			  src/patterns.c \
			  src/main.c \
			  src/cylinder.c \
			  src/material.c

OBJ         = $(SRC:.c=.o)

INC         = -I./inc/

HEADERS		=	inc/raytracing.h

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

$(NAME): $(MLX_LIB) $(OBJ) $(HEADERS)
	@echo "Linking..."
	@$(CXX) $(CXXFLAGS) $(OBJ) $(MLX_O_FLAGS) -o $(NAME) -lm -lSDL2
	@echo "Executable created!"

%.o: %.c $(HEADERS)
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

.PHONY: all clean fclean re