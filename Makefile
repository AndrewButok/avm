# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: abutok <marvin@42.fr>                      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/07/23 18:03:27 by abutok            #+#    #+#              #
#    Updated: 2019/07/25 12:07:55 by abutok           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Project name
NAME = Abstract-VM
# C++ compiler with flags
COMPILER = clang++ -Wall -Werror -Wextra -std=c++11
# Includes
INCLUDEDIR = ./src/includes/
INCLUDEFILES  = IOperand.hpp Operand.hpp OperandFactory.hpp Executor.hpp
INCLUDE = $(addprefix $(INCLUDEDIR), $(INCLUDEFILES))
BOOSTINCLUDE = /Users/abutok/.brew/opt/boost/include
# Sources
SRCDIR = ./src/
SRC = OperandFactory.cpp Executor.cpp main.cpp
# Binaries
BINDIR = ./out/
BIN = $(addprefix $(BINDIR), $(SRC:.cpp=.o))

.NOTPARALLEL = all fclean clean re $(NAME)

all: $(NAME)

$(NAME): $(BINDIR) $(BIN)
	$(COMPILER) -o $(NAME) -I $(INCLUDEDIR) -I $(BOOSTINCLUDE) $(BIN)

$(BINDIR):
	@if [ ! -d "$(BINDIR)" ]; then mkdir $(BINDIR); fi

$(BINDIR)%.o: $(SRCDIR)%.cpp $(INCLUDE)
	$(COMPILER) -c -I $(INCLUDEDIR) -I $(BOOSTINCLUDE) $< -o $@

clean:
	if [ -d "$(BINDIR)" ]; then rm -rf $(BINDIR); fi

fclean: clean
	if [ -f "$(NAME)" ]; then rm -rf $(NAME); fi

re: fclean all