/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline_parser5.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaratzi <mkaratzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 18:05:49 by mkaratzi          #+#    #+#             */
/*   Updated: 2023/06/30 18:33:47 by mkaratzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_in_fd(t_cmd_pre *cmd, char *line, int i)
{
	int		in_fd;
	char	*holder;

	holder = NULL;
	in_fd = -3;
	line[i++] = ' ';
	if (line[i] == '<')
	{
		line[i] = ' ';
		in_fd = create_heredoc(line, 0, 0);
		cmd->stopped_heredoc = in_fd;
	}
	else
	{
			holder = get_next_arg(line, 0, 0);
			in_fd = open(holder, O_RDONLY, 0666);
	}
	if (in_fd == -1)
		in_fd = -2;
	cmd->in_fd = in_fd;
	free(holder);
	return (EXIT_SUCCESS);
}

static char	*replace_env_helper(char **ptrs, char *str)
{
	char	*found;
	int		i;

	found = NULL;
	i = -1;
	while (ptrs[++i] && !found)
		if (!env_compare(str, ptrs[i]))
			found = ptrs[i];
	return (found);
}

static int	replace_env_writer(char *found, int fd, int *size)
{
	int	i;

	i = 0;
	while (found != NULL && found[i] && found[i] != '=')
		i++;
	while (found != NULL && found[++i])
		write_and_count(fd, found[i], size);
	return (EXIT_SUCCESS);
}

int	replace_env(const char *str, int fd, char **ptrs, int *size)
{
	char	buffer[PATH_MAX];
	char	*found;
	int		k;
	int		i;

	if (str[1] == '?')
		return (question_mark_found(fd, size, ptrs[0]));
	if (str[1] != '_' && !ft_isalnum(str[1]))
		return (1);
	i = ft_strlen(str);
	ft_bzero(&buffer, PATH_MAX);
	buffer[0] = str[0];
	k = 0;
	found = NULL;
	while (str[++k] && str[k] != ' ' && (ft_isalnum(str[k]) || str[k] == '_'))
		buffer[k] = str[k];
	buffer[k] = '\0';
	found = replace_env_helper(ptrs, &buffer[1]);
	replace_env_writer(found, fd, size);
	if (found == NULL && k == 1)
		write_and_count(fd, buffer[0], size);
	return (k);
}
