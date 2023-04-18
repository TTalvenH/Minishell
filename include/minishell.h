/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaratzi <mkaratzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 01:12:28 by mkaratzi          #+#    #+#             */
/*   Updated: 2023/04/18 17:14:38 by mkaratzi         ###   ########.fr       */
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
# include "../libft/include/get_next_line.h"
# include "../libft/include/libft.h"
# define NO_EQUAL_SIGN 0
# define EQUAL_SIGN 1

typedef struct s_cmd_pre{
	char	**args;
	char	**redirects;
}	t_cmd_pre;

typedef struct s_env
{
	char			env[200];
	struct s_env	*next;
}	t_env;

typedef struct s_new_line
{
	char				**exec_lines;
	t_env				*environments;
	char				**envs_pointers;
	t_cmd_pre			**cmd_pre;
	int					length;
	int					line_count;
	int					output_fd;
	short				exit_req;
}	t_new_line;

typedef struct s_pipe_chain
{	
	int		**pipe_fds;
	pid_t	*pids;
}	t_pipe_chain;

// environment functions
int		get_environments(t_new_line *got_line);
int		free_all_env(t_new_line *new_line);
int		update_env(const char *env, t_env *new_env);
int		export_env(t_new_line *got_line, const char *export_env);
int		unset_env(t_new_line *got_line, const char *name);
int		llist_to_array(t_new_line *new_line);
int		env_compare(const char *env1, const char *env2, int instrcution);
int		valid_identifier(const char *export_env);

//smart history
int		get_history_path(char path_to_history_file[24]);
int		get_history(char path_to_history_file[24]);
int		add_to_history(char *str, char path_to_history_file[24]);

//parsing
int		read_line_parser(char *line, t_new_line *got_line);
int		count_substrings(char *str);
int		assign_pointers(char *str, t_new_line *got_line, int i);
int		free_got_line(t_new_line *got_line);
int		assign_cmd_pre(t_new_line *got_line);
int		count_cmd_pointers(const char *str, int *c_args, int *c_redirects);

//handling 
int		word_compare(char *exec_line, char *word);
int		has_builtin(char *exec_line);
void	line_handling_func(t_new_line *handle_line);

//minishell_utils
char	check_quotes(char *str, int i, char expecting);
int		free_got_line(t_new_line *got_line);
int		skip_redirect(const char *str, int key, int	k, int	i);
int		skip_quotes(const char *str);

//piping
int		piping(t_new_line *got_line);

//!poista
void	print_2d_array(char **arr);

#endif
