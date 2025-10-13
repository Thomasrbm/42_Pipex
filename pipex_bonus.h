/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 23:06:56 by throbert          #+#    #+#             */
/*   Updated: 2025/10/13 05:57:11 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

# include "42_GNL/get_next_line_bonus.h"
# include "libft/libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <unistd.h>

# define PATH_LENGTH 5
# define EXEC_FILES_AND_LASTCMD 4
# define EXEC_FILE_HD_LIM_AND_LASTCMD 5
# define LASTCMD 1
# define SKIP_PIPEX_AND_INFILE 2
# define SKIP_PIPEX_HEREDOC_AND_DELIMITER 3
# define WRITING_1 1
# define READING_0 0
# define STDOUT 1
# define CMD_NOT_EXECUTABLE 126
# define ERROR 1

typedef struct s_shared
{
	int		**pipefd;
	char	**env;
	char	*path;
	int		pipecount;
	int		here_doc;
	int		here_doc_pipe[2];
	pid_t	*pid;
	int		cmd_count;
}			t_shared;

void		ft_redirect(char *redirect, char *file);
int			wait_for_children(pid_t *pid, int cmd_count);
int			ft_putstr_fd_int(char *s, int fd);
int			init_shared(struct s_shared *shared, char **env, int ac, char **av);
char		*get_path(char **env);
void		clean_and_exit(char **cmd, char *abs_path, int exit_code);
char		*ft_strjoin3(char *s1, char *s2, char *s3);
void		close_all_pipes(t_shared *sh);
void		cleanup_exit(char **cmd, struct s_shared shared, int exit_code);
int			handle_here_doc(struct s_shared *shared, char *limiter);
char		*get_abs_path(char *path, char *cmd);
int			execute_commands(struct s_shared *shared, char **av, int i);
void		free_pipes(t_shared *sh);

#endif