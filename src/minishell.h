/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaratzi <mkaratzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 01:12:28 by mkaratzi          #+#    #+#             */
/*   Updated: 2023/04/14 07:13:58 by mkaratzi         ###   ########.fr       */
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
# include "get_next_line.h"

typedef struct s_new_line
{
	char	**exec_lines;
	char	*string;
	int		length;
	int		line_count;
}	t_new_line;

//generally used from HIVE
size_t	ft_strlen(const char *s);
size_t	ft_strlcpy(char *dst, const char *src, size_t size);
char	*get_next_line(int fd);

//smart history
int		get_history_path(char path_to_history_file[24]);
int		get_history(char path_to_history_file[24]);
int		add_to_history(char *str, char path_to_history_file[24]);

//parsing
int		read_line_parser(char *line, t_new_line *got_line);
int		count_substrings(char *str);
int		assign_pointers(char *str, t_new_line *got_line);
int		free_got_line(t_new_line *got_line);

#endif
