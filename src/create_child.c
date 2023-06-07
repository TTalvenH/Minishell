/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_child.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttalvenh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/03 12:56:11 by ttalvenh          #+#    #+#             */
/*   Updated: 2023/06/03 12:56:13 by ttalvenh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/errno.h>
#include <unistd.h>
#include <signal.h>


char	*our_getenv(char *str, t_new_line *got_line)
{
	int	i;
	int	str_len;

	str_len = ft_strlen(str);
	i = 0;
	if (!str)
		return (NULL);
	while (got_line->our_environ[i])
	{
		if (!ft_strncmp(str, got_line->our_environ[i], str_len))
			return (got_line->our_environ[i]);
		i++;
	}
	return (NULL);
}

char	*find_cmd_path(char *cmd, t_new_line *got_line)
{
	int			i;
	char		**paths;
	char		*cmd_path;

	i = 0;
	if (!cmd || !*cmd)
		return (NULL);
	if ((cmd[0] == '/' && !access(cmd, F_OK)))
		return (cmd);
	paths = ft_split(our_getenv("PATH", got_line), ':');
	while (paths[i] != NULL)
	{
		cmd_path = ft_strjoin_slash(paths[i++], cmd);
		if (access(cmd_path, F_OK) == 0)
		{
			ft_free_array(paths);
			return (cmd_path);
		}
		free(cmd_path);
	}
	ft_free_array(paths);
	return (NULL);
}

int	child_process(char **arg, t_pipe_chain *pipes, t_new_line *got_line)
{
	char		*cmd_path;

	cmd_path = NULL;
	if (dup2(pipes->in_fd, STDIN_FILENO) < 0)
		return (-1);
	if (dup2(pipes->out_fd, STDOUT_FILENO) < 0)
		return (-1);
	close_pipes(pipes);
	cmd_path = find_cmd_path(arg[0], got_line);
	if (got_line->builtin == 0
		&& (cmd_path == NULL || pipes->in_fd == -2 || pipes->out_fd == -2))
		ft_printf_fd(2, "Bad command/Broken I/O: %s\n", arg[0]);
	else if (got_line->builtin == 0)
		execve(cmd_path, arg, got_line->our_environ);
	else
		exit(run_builtin(arg, pipes, got_line));
	exit(-1);
}

pid_t	create_child(char **arg, t_pipe_chain *pipes, t_new_line *got_line)
{
	pid_t		pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(0);
	}
	if (pid == 0)
		child_process(arg, pipes, got_line);
	return (pid);
}
