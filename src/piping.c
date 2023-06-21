/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   piping.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaratzi <mkaratzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/03 12:56:17 by ttalvenh          #+#    #+#             */
/*   Updated: 2023/06/20 09:27:31 by mkaratzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/errno.h>
#include <unistd.h>
#include <signal.h>

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

int	pipe_logic(t_pipe_chain *pipes, t_new_line *got_line)
{
	int	i;

	i = 0;
	while (i <= pipes->pipe_count)
	{
		got_line->builtin = has_builtin(got_line->cmd_pre[i].args[0]);
		set_io_fd(got_line, pipes, i);
		if (got_line->builtin < 0)
			break ;
		if (pipes->pipe_count || !got_line->builtin)
			pipes->pids[i]
				= create_child(got_line->cmd_pre[i].args, pipes, got_line);
		else
		{
			return (
				handle_builtins(got_line->cmd_pre[i].args, pipes, got_line));
		}
		i++;
	}
	return (0);
}

int	piping(t_new_line *got_line)
{
	int				i;
	int				status;
	t_pipe_chain	pipes;

	i = 0;
	ft_bzero(&pipes, sizeof(t_pipe_chain));
	pipes.pipe_count = got_line->line_count - 1;
	if (init_pipes(&pipes))
		return (1);
	status = pipe_logic(&pipes, got_line);
	if (status)
		return (status);
	if (pipes.pipe_count)
		close_pipes(&pipes, got_line->copy);
	while (pipes.pids[i])
		waitpid(pipes.pids[i++], &status, 0);
	return (WEXITSTATUS(status));
}
