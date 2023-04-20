#include "minishell.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/errno.h>
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

int		init_pipes(t_pipe_chain *pipes)
{
	int i;
	
	i = 0;
	if(pipes->pipe_count < 0)
		return(1);
	pipes->pipe_fds = malloc (sizeof(int *) * (pipes->pipe_count));
	pipes->pids = malloc (sizeof(pid_t) * pipes->pipe_count + 1); 
	if (!pipes->pipe_fds || !pipes->pids)
		return (1);
	while (i < pipes->pipe_count)
	{
		pipes->pipe_fds[i] = malloc(sizeof(int) * 2);
		if (pipe(pipes->pipe_fds[i++]) < 0)
			return (1);
	}
	return (0);

}

int	child_execve(char **arg, int input_fd, int output_fd, t_pipe_chain *pipes)
{
	extern char **environ;
	int	pid;
	pid = fork();
	char *cmd_path;
	cmd_path = NULL;
	if (pid == -1)
	{
		perror("fork");
		exit(0);
	}
	if (pid == 0 && (input_fd >= 0 && output_fd >= 0))
	{
		ft_printf("%s\n", find_cmd_path(arg[0]));
		dup2(input_fd, STDIN_FILENO);
		dup2(output_fd, STDOUT_FILENO);
		if (input_fd != 0)
			close(input_fd);
		if (output_fd != 1)
			close(output_fd);
		cmd_path = find_cmd_path(arg[0]);
		if (cmd_path == NULL)
			ft_printf_fd(2, "Bad command\n");
		else
			execve(cmd_path, arg, environ);
		printf("test: %s\n", strerror(errno));
		close_pipes(pipes);
		exit(-1);
	}
	return (pid);
}

int	piping(t_new_line *got_line)
{
	char	*test1[] = {"echo", "test\n", NULL};
	char	*test4[] = {"wc", "-l", NULL};
	char	*test3[] = {"cat", "-e", NULL};
	char	*test2[] = {"grep", "test", NULL};
	char	**test_arr2[] = {test1, test2, test3, test4, NULL};
	int				i;
	t_pipe_chain	pipes;

	pipes.pipe_count = got_line->line_count - 1;
	i = 0;
	if (pipes.pipe_count && init_pipes(&pipes) < 0)
		return (1);
	while (i <= pipes.pipe_count)
	{
		if (!pipes.pipe_count)
			child_execve(test_arr2[i], 0, 1, &pipes);
		else if (!i)
			child_execve(test_arr2[i], 0, pipes.pipe_fds[i][1], &pipes);
		else if (i == pipes.pipe_count)
			child_execve(test_arr2[i], pipes.pipe_fds[i - 1][0], 1, &pipes);
		else
			child_execve(test_arr2[i], pipes.pipe_fds[i - 1][0],
				 pipes.pipe_fds[i][1], &pipes);
		i++;
	}
	if (pipes.pipe_count)
		close_pipes(&pipes);
	return (0);
}