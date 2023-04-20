#include "minishell.h"
#include <stdio.h>
#include <unistd.h>

int		init_pipes(t_pipe_chain *pipes, t_new_line *got_line)
{
	int i;
	
	i = 0;
	if(!got_line->line_count)
		return(1);
	pipes->pipe_fds = malloc (sizeof(int *) * (got_line->line_count - 1));
	pipes->pids = malloc (sizeof(pid_t) * got_line->line_count); 
	if (!pipes->pipe_fds || !pipes->pids)
		return (1);
	while (i < got_line->line_count - 1)
	{
		pipes->pipe_fds[i] = malloc(sizeof(int) * 2);
		if (pipe(pipes->pipe_fds[i++]) < 0)
			return (1);
	}
	return (0);

}

// int	child_execve(char **arg, int input_fd, int output_fd)
// {
// 	int	pid;

// 	pid = fork();
// 	if (pid == -1)
// 	{
// 		perror("fork");
// 		exit(0);
// 	}
// 	if (pid == 0 && (input_fd >= 0 && output_fd >= 0))
// 	{
// 		dup2(input_fd, STDIN_FILENO);
// 		dup2(output_fd, STDOUT_FILENO);

// 		execve(find_cmd_path(arg[0]), arg, 0);
// 		ft_printf_fd(2, "pipex: %s: %s\n", strerror(errno), arg[0]);
// 		exit(-1);
// 	}
// 	return (pid);
// }

// int	piping(t_new_line *got_line)
// {
// 	t_pipe_chain pipes;
// 	int i;
// 	if (init_pipes(&pipes, got_line) < 0)
// 		return (1);
// 	while (i < got_line->line_count)
// 	{
// 		if (!i)
// 			child_execve(got_line->cmd_pre.args, 0, pipes.pipe_fds[i][1])
// 		child_execve(got_line->cmd_pre.args, )
// 	}
// 	return (0);
// }