/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 23:05:51 by throbert          #+#    #+#             */
/*   Updated: 2025/10/03 21:09:34 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

// LE PATH precis
char	*get_abs_path(char *path, char *cmd, struct s_shared shared)
{
	char	**to_try;
	char	*merged_try;
	int		i;

	(void) shared;
	i = 0;
	to_try = ft_split(path, ':');
	if (!to_try)
		return (NULL);
	while (to_try[i])
	{
		merged_try = ft_strjoin3(to_try[i], "/", cmd);
		if (access(merged_try, X_OK) == 0)
		{
			free_split(to_try);
			return (merged_try);
		}
		free(merged_try);
		i++;
	}
	free_split(to_try);
	return (NULL);
}

void	ft_exec_cmd1(char *absolute_path, char **cmd,
char *file, struct s_shared shared)
{
	int	infile;

	infile = open(file, O_RDONLY);
	if (infile < 0)
		clean_and_exit(cmd, absolute_path, ERROR);
	dup2(infile, STDIN_FILENO);
	dup2(shared.pipefd[WRITING_1], STDOUT_FILENO);
	close(infile);
	close(shared.pipefd[READING_0]); // close les deux pck ouvert les deux lors de la commande pipe,  on peut les close car la duplication fait que les stdin et out pointe vers elles et ca ne cause pas de fd ouvert. 
	close(shared.pipefd[WRITING_1]);
	if (execve(absolute_path, cmd, shared.env) == -1)
	{
		perror("execve");
		clean_and_exit(cmd, absolute_path, CMD_NOT_EXECUTABLE);
	}
}

void	ft_exec_cmd2(char *absolute_path, char **cmd,
	char *file, struct s_shared shared)
{
	int	outfile;

	outfile = open(file, O_CREAT | O_WRONLY | O_TRUNC, 0644); // OU binaire, ca va activer tous les flags 1111. (ils sont 0100 0010 0001)  = 0111
	if (outfile < 0)
		clean_and_exit(cmd, absolute_path, ERROR);
	// old fd, newfd. 
	// newfd partage le meme fichier que old fd.  ils pointent au meme endroit
	// stdout marche toujours mais newfd aussi peut y avoir acces. 

	// fichier de droite est redirect vers fichier de gauche.

	// je vais launch ./pipex, il va use le stdin MAIS il a ete redirect et ainsi de suite. 
	dup2(shared.pipefd[READING_0], STDIN_FILENO);
	dup2(outfile, STDOUT_FILENO);
	close(outfile);
	close(shared.pipefd[READING_0]);
	close(shared.pipefd[WRITING_1]);  // si celui la pas close = boucle inf ===> car en cas de cmd qui lit (cat ou wc) la cmd 1 n evoit jamais EOF donc cmd ne peut jamais commencer a lire. pck l ecriture n est jamais fini
	if (execve(absolute_path, cmd, shared.env) == -1)
	{
		perror("execve");
		clean_and_exit(cmd, absolute_path, CMD_NOT_EXECUTABLE);
	}
}

int	execute_commands(char *cmd_str, char *arg, struct s_shared shared, int cmd_type)
{
	pid_t	pid;
	char	**cmd;
	char	*absolute_path;

	pid = fork();
	if (pid == 0)
	{
		cmd = ft_split(cmd_str, ' ');
		if (!cmd || !cmd[0])
			cleanup_exit(cmd, shared, 1);
		absolute_path = get_abs_path(shared.path, cmd[0], shared);
		if (!absolute_path)
			cleanup_exit(cmd, shared, 1);
		if (cmd_type == 1)
			ft_exec_cmd1(absolute_path, cmd, arg, shared);
		else
			ft_exec_cmd2(absolute_path, cmd, arg, shared);
	}
	return (pid);
}

int	main(int ac, char **av, char **env)
{
	pid_t		pid[2];
	t_shared	shared;

	if (ac != 5)
		return (ft_putstr_fd_int("Wrong number of arguments", 2));
	if (init_shared(&shared, env))
		return (1);
	pid[0] = execute_command(av[2], av[1], shared, 1);
	pid[1] = execute_command(av[3], av[4], shared, 2);
	close_pipes(&shared); // les fds tracker sont seulement ceux du parents sauf si add un flag track children 
	return (wait_for_children(pid));  // sinon les prog se font l un apres l autre + des leaks (sous valgrind)

}
