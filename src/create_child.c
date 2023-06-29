/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_child.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaratzi <mkaratzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/03 12:56:11 by ttalvenh          #+#    #+#             */
/*   Updated: 2023/06/29 17:10:07 by mkaratzi         ###   ########.fr       */
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

char	*relative_path(char *cmd)
{
	char	cwd[PATH_MAX];
	char	*cmd_path;

	cmd_path = NULL;
	if (!getcwd(cwd, sizeof(cwd)))
		return (NULL);
	cmd_path = ft_strjoin_slash(cwd, cmd);
	if (!access(cmd_path, F_OK))
		return (cmd_path);
	else
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
	else if (!ft_strncmp(cmd, "./", 2) || !ft_strncmp(cmd, "../", 3))
		return (relative_path(cmd));
	paths = ft_split(our_getenv("PATH", got_line) + 5, ':');
	while (paths[i] != NULL)
	{	
		cmd_path = ft_strjoin_slash(paths[i++], cmd);
		if (!access(cmd_path, F_OK))
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
	dup2(got_line->copy, STDIN_FILENO);
	reset_sig(SIGINT);
	if (dup2(pipes->in_fd, STDIN_FILENO) < 0)
		pipes->in_fd = -2;
	if (dup2(pipes->out_fd, STDOUT_FILENO) < 0)
		pipes->out_fd = -2;
	close_pipes(pipes, -1);
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
