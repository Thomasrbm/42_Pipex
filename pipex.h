/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 23:06:56 by throbert          #+#    #+#             */
/*   Updated: 2025/10/13 05:56:38 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#define PATH_LENGTH 5

#define WRITING_1 1
#define READING_0 0
#define STDOUT 1
#define CMD_NOT_EXECUTABLE 126
#define ERROR 1

typedef struct s_shared
{
	int		pipefd[2];
	char	**env;
	char	*path;
}			t_shared;

void		ft_redirect(char *redirect, char *file);
int			wait_for_children(pid_t *pid);
int			ft_putstr_fd_int(char *s, int fd);
int			init_shared(struct s_shared *shared, char **env);
char		*get_path(char **env);
void		clean_and_exit(char **cmd, char *abs_path, int exit_code);
char		*ft_strjoin3(char *s1, char *s2, char *s3);
void		close_pipes(struct s_shared *shared);
void		cleanup_exit(char **cmd, struct s_shared shared, int exit_code);
int			execute_commands(char *cmd_str, char *arg, struct s_shared shared,
				int cmd_type);
void		cmd_not_found(char **cmd, struct s_shared shared);
