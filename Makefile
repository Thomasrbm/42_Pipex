NAME = pipex

CC = cc
CFLAGS = -Wall -Wextra -Werror -g3

OBJDIR = ./objects

SRCS = main.c \
	   utils.c \
	   clean_utils.c \
	   libft/printers/ft_putstr_fd.c \
	   libft/comparers/ft_strncmp.c \
	   libft/splitters/ft_substr.c \
	   libft/splitters/ft_split.c \
	   libft/splitters/ft_strjoin.c \
	   libft/splitters/ft_strlcpy.c \
	   libft/sizers/ft_strlen.c \
	   libft/allocs/ft_strdup.c \
	   libft/allocs/ft_memcpy.c \
	   libft/allocs/free_split.c \
	   libft/searchers/ft_strchr.c \

OBJS = $(patsubst %.c,$(OBJDIR)/%.o,$(SRCS))

RM = rm -f

all: libft $(NAME)
	@echo "Compiled successfully!"
	@EXEC_LINE="=> execute ./$(NAME) [Input file] [cmd 1] [cmd 2] [output file]"; \
	LEN=$$(echo "$$EXEC_LINE" | wc -c); \
	LEN=$$((LEN - 1)); \
	BORDER_LEN=$$((LEN + 2)); \
	TOP="┌$$(printf '─%.0s' $$(seq 1 $$BORDER_LEN))┐"; \
	MID="│ $$EXEC_LINE │"; \
	BOT="└$$(printf '─%.0s' $$(seq 1 $$BORDER_LEN))┘"; \
	echo "\033[35m$$TOP"; \
	echo "$$MID"; \
	echo "$$BOT\033[0m"

libft:
	@if [ ! -d "libft" ]; then \
		echo "\033[33mCloning libft repository...\033[0m"; \
		git clone git@github.com:Thomasrbm/42_Turbo-Libft.git libft; \
		echo "\033[32mLibft cloned successfully!\033[0m"; \
	else \
		echo "\033[33mLibft directory already exists.\033[0m"; \
	fi

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

$(OBJDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@if [ -d "$(OBJDIR)" ]; then \
		echo "\033[33mCleaning object files...\033[0m"; \
		$(RM) -r $(OBJDIR); \
		echo "\033[32mClean completed successfully!\033[0m"; \
	else \
		echo "\033[33mNo objects to clean.\033[0m"; \
	fi

fclean: clean
	@if [ -f "$(NAME)" ]; then \
		echo "\033[33mRemoving $(NAME)...\033[0m"; \
		$(RM) $(NAME); \
		echo "\033[32mFull clean completed successfully!\033[0m"; \
	else \
		echo "\033[33mNo executable to clean.\033[0m"; \
	fi

cleanlib:
	@if [ -d "libft" ]; then \
		echo "\033[33mRemoving libft directory...\033[0m"; \
		$(RM) -r libft; \
		echo "\033[32mLibft directory removed successfully!\033[0m"; \
	else \
		echo "\033[33mNo libft directory to clean.\033[0m"; \
	fi

re: fclean all

.PHONY: all libft clean fclean libclean re
