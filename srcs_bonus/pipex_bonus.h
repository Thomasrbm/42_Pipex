/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 19:08:35 by marvin            #+#    #+#             */
/*   Updated: 2025/10/16 03:20:42 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#define READ_0 0
#define WRITE_1 1
#define SKIP_PIPEX_INOUTFILES 3
#define SKIP_PIPEX_INFILE 2
#define SKIP_PIPEX_HD_DEL 3
#define SKIP_PIPEX_HD_DEL_OUTFILE 4
#define LAST_CMD 1
#define CMD_NOT_FOUND 127
#define CMD_NOT_EXECUTABLE 126

#include "../42_GNL/get_next_line_bonus.h"
#include "../libft/libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

typedef struct s_shared
{
	char	**env;
	char	*path;
	int		heredoc_flag;
	char	*delim;
	int		heredoc_pipes[2];
	int		cmd_count;
	int		pipecount;
	int		**pipefd;
	pid_t	*pid_tab;
	int		status;
	char	*infile;
	char	*outfile;
	char	*abs_path;
	char	**cmd;
}			t_shared;

int			init_shared(t_shared *shared, int ac, char **av, char **env);
void		exec_cmds(t_shared *shared, char **av);
int			wait_for_childrend(t_shared *shared);
void		clean_exit(t_shared *shared, char **cmd, int exit_code);
void		handle_heredoc(t_shared *shared);
void		close_all_pipes(t_shared *shared);
void		free_all(t_shared *shared, char **cmd);
void		cmd_not_found(char **cmd, t_shared *shared);
