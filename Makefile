NAME = pipex
BONUS_NAME = pipex_bonus

CC = cc
CFLAGS = -Wall -Wextra -Werror -g3
RM = rm -f

OBJDIR = ./objects
BONUS_DIR = ./objects_bonus

# ---------------------------------------------------------------------------
# SRCS
# ---------------------------------------------------------------------------
SRCS = pipex.c \
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

BONUS_SRCS = pipex_bonus.c utils_bonus.c here_doc_bonus.c clean_utils_bonus.c \
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
BONUS_OBJS = $(patsubst %.c,$(BONUS_DIR)/%.o,$(BONUS_SRCS))

# ---------------------------------------------------------------------------
# LIBS
# ---------------------------------------------------------------------------
GNL_DIR  = 42_GNL
GNL_REPO = git@github.com:Thomasrbm/42_GNL.git
GNL_LIB  = $(GNL_DIR)/get_next_line.a

# ---------------------------------------------------------------------------
# RULES: NORMAL
# ---------------------------------------------------------------------------
all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS)
	@$(MAKE) --no-print-directory show_msg

libft:
	@if [ ! -d "libft" ]; then \
		echo "\033[33mCloning libft repository...\033[0m"; \
		git clone git@github.com:Thomasrbm/42_Turbo-Libft.git libft; \
		echo "\033[32mLibft cloned successfully!\033[0m"; \
	else \
		echo "\033[33mLibft directory already exists.\033[0m"; \
	fi

$(OBJDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

show_msg:
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

# ---------------------------------------------------------------------------
# RULES: BONUS
# ---------------------------------------------------------------------------
# ---------------------------------------------------------------------------
# RULES: BONUS
# ---------------------------------------------------------------------------
bonus: $(BONUS_NAME)

$(BONUS_NAME): $(BONUS_OBJS)
	@if [ ! -d "$(GNL_DIR)" ]; then \
		echo "\033[33mCloning GNL repository...\033[0m"; \
		git clone $(GNL_REPO) $(GNL_DIR); \
		echo "\033[32mGNL cloned successfully!\033[0m"; \
	else \
		echo "\033[33mGNL directory already exists.\033[0m"; \
	fi
	@$(CC) $(CFLAGS) -c $(GNL_DIR)/get_next_line.c -o $(GNL_DIR)/get_next_line.o
	@ar rcs $(GNL_LIB) $(GNL_DIR)/get_next_line.o
	@echo "\033[36mBuilding pipex bonus...\033[0m"
	@$(CC) $(CFLAGS) $(BONUS_OBJS) $(GNL_LIB) -o $(BONUS_NAME)
	@echo "\033[32m✅ pipex_bonus built successfully!\033[0m"

$(BONUS_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

fclean_bonus:
	@if [ -f "$(BONUS_NAME)" ]; then \
		echo "\033[33mRemoving $(BONUS_NAME)...\033[0m"; \
		$(RM) $(BONUS_NAME); \
		echo "\033[32mBonus executable removed successfully!\033[0m"; \
	else \
		echo "\033[33mNo bonus executable to clean.\033[0m"; \
	fi
	@if [ -d "$(BONUS_DIR)" ]; then \
		$(RM) -r $(BONUS_DIR); \
	fi
	@if [ -d "$(GNL_DIR)" ]; then \
		echo "\033[33mRemoving 42_GNL directory...\033[0m"; \
		$(RM) -r $(GNL_DIR); \
		echo "\033[32m42_GNL directory removed successfully!\033[0m"; \
	else \
		echo "\033[33mNo 42_GNL directory to clean.\033[0m"; \
	fi


.PHONY: all show_msg libft clean fclean cleanlib re bonus fclean_bonus
