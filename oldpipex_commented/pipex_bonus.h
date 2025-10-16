/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 23:06:56 by throbert          #+#    #+#             */
/*   Updated: 2025/10/16 02:36:14 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

# include "../42_GNL/get_next_line_bonus.h"
# include "../libft/libft.h"
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
# define WRITE_1 1
# define READ_0 0
# define STDOUT 1
# define CMD_NOT_EXECUTABLE 126
# define CMD_NOT_FOUND 127
# define ERROR 1

typedef struct s_shared
{
	int		**pipefd; // deux tab, celui vers chaque pipe + chaque pipe est un tab
	char	**env;
	char	*path;
	int		pipecount;
	int		here_doc_flag;
	int		here_doc_pipe[2];
	pid_t	*pid; // juste un tab de pleins d int
	int		cmd_count;
}			t_shared;

void		ft_redirect(char *redirect, char *file);
int			wait_for_children(pid_t *pid, int cmd_count);
int			ft_putstr_fd_int(char *s, int fd);
int			init_shared(struct s_shared *shared, char **env, int ac, char **av);
char		*get_path(char **env);
char		*ft_strjoin3(char *s1, char *s2, char *s3);
void		close_all_pipes(t_shared *sh);
int			handle_here_doc(struct s_shared *shared, char *limiter);
char		*get_abs_path(char *path, char *cmd);
int			execute_commands(struct s_shared *shared, char **av, int i);
void		free_pipes_tab(t_shared *sh);
void		clean_and_exit_child(char **cmd, char *abs_path, int exit_code);




#endif