/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 23:05:51 by throbert          #+#    #+#             */
/*   Updated: 2025/10/13 04:06:07 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char    *get_abs_path(char *path, char *cmd, struct s_shared shared)
{
    char    **to_try;
    char    *merged_try;
    int     i;
    
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

void    ft_exec_cmd1(char *absolute_path, char **cmd,
    char *file, struct s_shared shared)
{
    int infile;
    
    infile = open(file, O_RDONLY);
    if (infile < 0)
    {
        close(shared.pipefd[READING_0]);
        close(shared.pipefd[WRITING_1]);
        perror("open infile");
        clean_and_exit(cmd, absolute_path, ERROR);
    }
    dup2(infile, STDIN_FILENO);
    dup2(shared.pipefd[WRITING_1], STDOUT_FILENO);
    close(infile);
    close(shared.pipefd[READING_0]);
    close(shared.pipefd[WRITING_1]);
    if (execve(absolute_path, cmd, shared.env) == -1)
    {
        perror("execve");
        clean_and_exit(cmd, absolute_path, CMD_NOT_EXECUTABLE);
    }
}

void    ft_exec_cmd2(char *absolute_path, char **cmd,
    char *file, struct s_shared shared)
{
    int outfile;
    
    outfile = open(file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (outfile < 0)
    {
        close(shared.pipefd[READING_0]);
        close(shared.pipefd[WRITING_1]);
        perror("open outfile");
        clean_and_exit(cmd, absolute_path, ERROR);
    }
    dup2(shared.pipefd[READING_0], STDIN_FILENO);
    dup2(outfile, STDOUT_FILENO);
    close(outfile);
    close(shared.pipefd[READING_0]);
    close(shared.pipefd[WRITING_1]);
    if (execve(absolute_path, cmd, shared.env) == -1)
    {
        perror("execve");
        clean_and_exit(cmd, absolute_path, CMD_NOT_EXECUTABLE);
    }
}

int execute_commands(char *cmd_str, char *arg,
    struct s_shared shared, int cmd_type)
{
    pid_t   pid;
    char    **cmd;
    char    *absolute_path;
    
    pid = fork();
    if (pid == 0)
    {
        cmd = ft_split(cmd_str, ' ');
        if (!cmd || !cmd[0])
        {
            close(shared.pipefd[READING_0]);
            close(shared.pipefd[WRITING_1]);
            cleanup_exit(cmd, shared, 1);
        }
        absolute_path = get_abs_path(shared.path, cmd[0], shared);
        if (!absolute_path)
        {
            close(shared.pipefd[READING_0]);
            close(shared.pipefd[WRITING_1]);
            ft_putstr_fd(cmd[0], 2);
            ft_putstr_fd(": command not found\n", 2);
            cleanup_exit(cmd, shared, 127);
        }
        if (cmd_type == 1)
            ft_exec_cmd1(absolute_path, cmd, arg, shared);
        else
            ft_exec_cmd2(absolute_path, cmd, arg, shared);
    }
    return (pid);
}

int main(int ac, char **av, char **env)
{
    pid_t       pid[2];
    t_shared    shared;
    
    if (ac != 5)
        return (ft_putstr_fd_int("Wrong number of arguments\n", 2));
    if (init_shared(&shared, env))
        return (1);
    pid[0] = execute_commands(av[2], av[1], shared, 1);
    pid[1] = execute_commands(av[3], av[4], shared, 2);
    close_pipes(&shared);
    return (wait_for_children(pid));
}
