/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 23:06:56 by throbert          #+#    #+#             */
/*   Updated: 2025/10/16 04:10:39 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../libft/libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#define READ_0 0
#define WRITE_1 1
#define SKIP_PIPEX_INOUTFILES 3
#define SKIP_PIPEX_INFILE 2
#define LAST_CMD 1
#define CMD_NOT_FOUND 127
#define CMD_NOT_EXECUTABLE 126

typedef struct s_shared
{
	int		pipefd[2];
	char	**env;
	char	**cmd;
	char	*path;
}			t_shared;

int			init_shared(t_shared *shared, char **env);
int			exec_cmds(t_shared *shared, char **av, int cmd_type);
int			wait_for_children(pid_t *pid);
void		clean_exit(t_shared *shared, char **cmd, int exit_code);
void		handle_heredoc(t_shared *shared);
void		close_pipes(t_shared *shared);
void		cmd_not_found(char **cmd, t_shared *shared);
