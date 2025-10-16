/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 23:07:43 by marvin            #+#    #+#             */
/*   Updated: 2025/10/16 02:12:11 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	handle_heredoc(t_shared *shared)
{
	char	*line;
	
	while (1)
	{
		write(1, "> ", 2);
		line = get_next_line(STDIN_FILENO);
		if (!line)
			break;
		// couper le \n pour comparaison
		// pck line add un \n
		if (ft_strncmp(line, shared->delim, ft_strlen(shared->delim)) == 0
			&& line[ft_strlen(shared->delim)] == '\n')
		{
			free(line);
			break;
		}
		write(shared->heredoc_pipes[WRITE_1], line, ft_strlen(line));
		free(line);
	}
}

