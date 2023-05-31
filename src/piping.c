/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   piping.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaratzi <mkaratzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 12:39:51 by ttalvenh          #+#    #+#             */
/*   Updated: 2023/05/31 23:25:57 by mkaratzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/errno.h>
#include <unistd.h>
#include <signal.h>

char	*find_cmd_path(char *cmd)
{
	int			i;
	char		**paths;
	char		*cmd_path;

	i = 0;
	if (!cmd || !*cmd)
		return (NULL);
	if ((cmd[0] == '/' && !access(cmd, F_OK)))
		return (cmd);
	paths = ft_split(getenv("PATH"), ':');
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

pid_t	child_exe(char **arg, t_pipe_chain *pipes, t_new_line *got_line)
{
	char		*cmd_path;
	pid_t		pid;

	pid = fork();
	cmd_path = NULL;
	if (pid == -1)
	{
		perror("fork");
		exit(0);
	}
	if (pid == 0)
	{
		dup2(pipes->in_fd, STDIN_FILENO);
		dup2(pipes->out_fd, STDOUT_FILENO);
		close_pipes(pipes);
		cmd_path = find_cmd_path(arg[0]);
		if (cmd_path == NULL || pipes->in_fd == -2 || pipes->out_fd == -2)
			ft_printf_fd(2, "Bad command/Broken I/O: %s\n", arg[0]);
		else
			execve(cmd_path, arg, got_line->our_environ);
		exit(-1);
	}
	return (pid);
}

void	set_io_fd(t_new_line *got_line, t_pipe_chain *pipes, int i)
{
	pipes->in_fd = 0;
	pipes->out_fd = 1;
	if (pipes->pipe_count && !i)
		pipes->out_fd = pipes->pipe_fds[i][1];
	else if (pipes->pipe_count && i == pipes->pipe_count)
		pipes->in_fd = pipes->pipe_fds[i - 1][0];
	else if (pipes->pipe_count)
	{
		pipes->in_fd = pipes->pipe_fds[i - 1][0];
		pipes->out_fd = pipes->pipe_fds[i][1];
	}
	if (got_line->cmd_pre[i].in_fd >= 0 || got_line->cmd_pre[i].in_fd == -2)
		pipes->in_fd = got_line->cmd_pre[i].in_fd;
	if (got_line->cmd_pre[i].out_fd >= 0 || got_line->cmd_pre[i].in_fd == -2)
		pipes->out_fd = got_line->cmd_pre[i].out_fd;
}

int	init_pipes(t_pipe_chain *pipes)
{
	int	i;

	i = 0;
	while (i < pipes->pipe_count)
	{
		if (pipe(pipes->pipe_fds[i++]))
			return (1);
	}
	return (0);
}

int	piping(t_new_line *got_line)
{
	int				i;
	int				j;
	t_pipe_chain	pipes;

	j = 0;
	i = 0;
	ft_bzero(&pipes, sizeof(t_pipe_chain));
	pipes.pipe_count = got_line->line_count - 1;
	if (init_pipes(&pipes))
		return (1);
	while (i <= pipes.pipe_count)
	{
		set_io_fd(got_line, &pipes, i);
		if (handle_builtins(got_line->cmd_pre[i].args, &pipes, got_line))
			pipes.pids[j++] = child_exe(got_line->cmd_pre[i].args,
					&pipes, got_line);
		i++;
	}
	if (pipes.pipe_count)
		close_pipes(&pipes);
	i = 0;
	while (pipes.pids[i])
	{
		waitpid(pipes.pids[i++], &j, 0);
	}
	return (j);
}
