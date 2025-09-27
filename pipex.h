/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 23:06:56 by throbert          #+#    #+#             */
/*   Updated: 2025/09/27 03:57:41 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#define PATH_LENGTH 5

typedef struct s_shared
{
	int		pipefd[2];
	char	**env;
	char	*path;
}	t_shared;

char	*alloc_cmd(char *cmd);
void	ft_redirect(char *redirect, char *file);
int		wait_for_children(pid_t *pid);
int		ft_putstr_fd_int(char *s, int fd);
int		init_shared(struct s_shared *shared, char **env);
char	*get_path(char **env);
