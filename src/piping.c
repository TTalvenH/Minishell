#include "minishell.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

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

int	piping(t_new_line *got_line)
{
	got_line = NULL;
	ft_printf("%s\n", find_cmd_path("cat"));
	// t_pipe_chain pipes;
	// int i;
	// if (init_pipes(&pipes, got_line) < 0)
	// 	return (1);
	// while (i < got_line->line_count)
	// {
	// 	if (!i)
	// 		child_execve(got_line->cmd_pre.args, 0, pipes.pipe_fds[i][1])
	// 	child_execve(got_line->cmd_pre.args, )
	// }
	return (0);
}