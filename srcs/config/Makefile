NAME= parse_test

VAL = -ggdb3
VAL_FLAG = --show-leak-kinds=all --leak-check=full --track-origins=yes
# comment $ASAN if dw use AddSan
# ASAN = -fsanitize=address -g3
CXXFLAG= -Wall -Wextra -Werror $(ASAN) -g3 -std=c++98 -pedantic -I../../includes

SRCS_FILE= \
	Config configServer configRoute \
	main helper
SRCS = $(addsuffix .cpp, $(SRCS_FILE))

OBJ_DIR = objs
OBJ:=$(SRCS:%.cpp=$(OBJ_DIR)/%.o)

$(OBJ_DIR)/%.o: %.cpp | $(OBJ_DIR)
	@c++ $(CXXFLAG) -c $< -o $@


all: $(NAME)

$(NAME): $(OBJ)
	@c++ $(CXXFLAG) $(OBJ) -o $(NAME)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

r: $(NAME)
	@./$(NAME)

#run with valgrind
v: CXXLAGS += $(VAL)
v: re $(OBJ)
	@c++ $(CXXFLAG) $(OBJ) -o $(NAME)
	@valgrind $VAL_FLAG ./$(NAME)

clean:
	@rm -rf $(OBJ_DIR)

fclean: clean
	@rm -rf $(NAME)

re: fclean all

.PHONY: all r v clean fclean re
