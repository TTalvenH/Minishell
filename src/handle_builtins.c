#include "minishell.h"

int	handle_builtins(char **args, t_pipe_chain *pipes, t_new_line *got_line)
{
	int	cmd;
	int	stdout;

	stdout = dup(STDOUT_FILENO);
	cmd = has_builtin(args[0]);
	if (cmd)
	{
		dup2(pipes->out_fd, STDOUT_FILENO);
		if (cmd == 1)
			echo(&args[1], NULL);
		else if (cmd == 2)
			cd(args[1]);
		else if (cmd == 3)
			pwd(NULL);
		else if (cmd == 4)
			export_env(args[1]);
		else if (cmd == 5)
			unset_env(args[1]);
		else if (cmd == 6)
			print_all_envs(got_line);
		else if (cmd == 7)
			exit(args[1]);
		dup2(stdout, STDOUT_FILENO);
		return (0);
	}
	return (1);
}