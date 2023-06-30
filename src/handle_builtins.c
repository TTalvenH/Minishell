/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_builtins.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaratzi <mkaratzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 17:31:30 by mkaratzi          #+#    #+#             */
/*   Updated: 2023/06/12 17:31:44 by mkaratzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	run_builtin(char **args, t_pipe_chain *pipes, t_new_line *got_line)
{
	int	status;

	status = 0;
	if (got_line->builtin == 1)
		status = echo(&args[1]);
	else if (got_line->builtin == 2)
		status = cd(args[1], got_line);
	else if (got_line->builtin == 3)
		status = pwd(NULL);
	else if (got_line->builtin == 4)
		status = our_export(&args[1], got_line);
	else if (got_line->builtin == 5)
		status = unset_env(&args[1]);
	else if (got_line->builtin == 6)
		status = print_all_envs(got_line, 1);
	else if (got_line->builtin == 7)
		status = exit_builtin(args, pipes, got_line->copy);
	if (got_line->line_count > 1)
		close(got_line->copy);
	free_got_line(got_line, NULL, 0);
	return (status);
}

int	handle_builtins(char **args, t_pipe_chain *pipes, t_new_line *got_line)
{
	int	stdout;
	int	status;

	stdout = dup(STDOUT_FILENO);
	pipes->copy_stdout = stdout;
	if (dup2(pipes->out_fd, STDOUT_FILENO) < 0)
		return (-1);
	status = run_builtin(args, pipes, got_line);
	if (dup2(stdout, STDOUT_FILENO) < 0)
		return (-1);
	close(stdout);
	return (status);
}
