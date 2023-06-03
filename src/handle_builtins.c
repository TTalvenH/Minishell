#include "minishell.h"

int	run_builtin(char **args, t_pipe_chain *pipes, t_new_line *got_line)
{
	int	status;

	status = 0;
	if (got_line->builtin == 1)
		status = echo(&args[1]);
	else if (got_line->builtin == 2)
		status = cd(args[1]);
	else if (got_line->builtin == 3)
		status = pwd(NULL);
	else if (got_line->builtin == 4)
		status = export_env(args[1], 0);
	else if (got_line->builtin == 5)
		status = unset_env(args[1]);
	else if (got_line->builtin == 6)
		status = print_all_envs(got_line);
	else if (got_line->builtin == 7)
		status = exit_builtin(args, pipes);
	return (status);
}

int	handle_builtins(char **args, t_pipe_chain *pipes, t_new_line *got_line)
{
	int	stdout;
	int	status;

	stdout = dup(STDOUT_FILENO);
	if (dup2(pipes->out_fd, STDOUT_FILENO) < 0)
		return (-1);
	status = run_builtin(args, pipes, got_line);
	if (dup2(stdout, STDOUT_FILENO) < 0)
		return (-1);
	return (status);
}
