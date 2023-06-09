/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaratzi <mkaratzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 01:12:28 by mkaratzi          #+#    #+#             */
/*   Updated: 2023/06/09 20:04:04 by mkaratzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <fcntl.h>
# include <unistd.h>
# include <stdlib.h>
# include <termios.h>
# include <sys/wait.h>
# include <signal.h>
# include "get_next_line.h"
# include "libft.h"
# define NO_EQUAL_SIGN 0
# define EQUAL_SIGN 1
# define PATH_MAX 1024


typedef struct s_cmd_pre{
	char				**args;
	int					in_fd;
	int					out_fd;
	int					stopped_heredoc;
}	t_cmd_pre;

typedef struct s_nums{
	int					i;
	int					expecting;
	int					size;
}	t_nums;

typedef struct s_env
{
	char			env[250];
	struct s_env	*next;
}	t_env;


typedef struct s_new_line
{
	char				**exec_lines;
	char				*parsed_line;
	char				*previous_result;
	char				**our_environ;
	t_cmd_pre			cmd_pre[100];
	int					length;
	int					line_count;
	int					output_fd;
	int					builtin;
	short				exit_req;
}	t_new_line;

typedef struct s_pipe_chain
{	
	int		pipe_fds[100][2];
	int		pipe_count;
	int		in_fd;
	int		out_fd;
	pid_t	pids[100];
}	t_pipe_chain;

extern t_env	*g_environ;

// environment functions
int		get_environments(void);
int		free_all_env(t_env *head);
int		update_env(const char *env, t_env *new_env);
int		export_env(const char *export_env, int instruction);
int		unset_env(char **name);
int		our_export(char	**args, t_new_line *got_line);
int		print_all_envs(t_new_line *got_line, int instruction);
int		llist_to_array(t_new_line *new_line);
int		env_compare(const char *env1, const char *env2);
int		valid_identifier(const char *export_env);

//smart history
int		copy_to_location(const char *str, char *dst);
int		get_history_path(char path_to_history_file[50]);
int		get_history(char path_to_history_file[50]);
int		add_to_history(char *str, char path_to_history_file[50]);


//parsing
int		read_line_parser(char *line, t_new_line *got_line);
int		count_substrings(char *str);
int		count_substring_return(int count, int expecting);
int		write_and_count(int fd, int character, int *size);
int		assign_pointers(char *str, t_new_line *got_line, int i);
int		free_got_line(t_new_line *got_line);
int		assign_cmd_pre(t_new_line *got_line);
char	*get_next_arg(char *str, int i, int len);
int		count_cmd_pointers(const char *str, int *c_args, int *c_redirects);
char	*initial_parse(const char *str, t_new_line *got_line);
int		initial_parse_loop(t_nums *m_n, int fd, const char *c,
			t_new_line *got_line);
int		fill_cmd_struct(char *line, t_cmd_pre *cmd, int ac);
int		get_out_fd(t_cmd_pre *cmd, char *line, int i, int out_fd);
int		get_in_fd(t_cmd_pre *cmd, char *line, int i);
char	*make_arg_string(char *str, int len, int i);
int		make_arg_string_loop(int i, int fd, int *len, char *str);
int		count_cmd_pointers(const char *str, int *c_args, int *c_redirects);
int		create_heredoc(char *line);
int		get_cmd_fds(t_cmd_pre *cmd, char *line, int i);
int		replace_env(const char *str, int fd, char **ptrs, int *size);
int		question_mark_found(int fd, int *size, const char *str);

//handling 
int		word_compare(char *exec_line, char *word, int instruction);
int		has_builtin(char *exec_line);
void	line_handling_func(t_new_line *handle_line);

//minishell_utils
int		close_pipes(t_pipe_chain *pipes);
char	check_quotes(char *str, int i, char expecting);
int		free_got_line(t_new_line *got_line);
int		skip_redirect(const char *str, int key, int k, int i);
int		skip_quotes(const char *str, int *counter, int *error);
char	*our_getenv(char *str, t_new_line *got_line);

//piping
int		piping(t_new_line *got_line);
int		handle_builtins(char **args, t_pipe_chain *pipes, t_new_line *got_line);
int		run_builtin(char **args, t_pipe_chain *pipes, t_new_line *got_line);
pid_t	create_child(char **arg, t_pipe_chain *pipes, t_new_line *got_line);



//builtins
int		cd(char	*dir, t_new_line *got_line);
int		pwd(char *args);
int		exit_builtin(char **args, t_pipe_chain *pipes);
int		echo(char **args);

#endif
