/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_history.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaratzi <mkaratzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 02:23:43 by mkaratzi          #+#    #+#             */
/*   Updated: 2023/04/14 02:56:59 by mkaratzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

int	copy_to_location(const char *str, char *dst)
{
	int	i;

	i = 0;
	while (str[i])
	{
		dst[i] = str[i];
		i++;
	}
	dst[i] = '\0';
	return (i);
}

int	get_history_path(char path_to_history_file[50])
{
	int					i;
	static const char	*file_name = "/.minishell_history";
	t_env				*head;

	i = 0;
	head = g_environ;
	while (head)
	{
		if (!env_compare(head->env, "HOME="))
			break ;
		head = head->next;
	}
	while (head->env[i++] != '=')
		;
	i = copy_to_location(&head->env[i], &path_to_history_file[0]);
	copy_to_location(file_name, &path_to_history_file[i]);
	return (EXIT_SUCCESS);
}

int	get_history(char path_to_history_file[50])
{
	char	*gnl;
	int		fd;

	get_history_path(path_to_history_file);
	fd = open(path_to_history_file, O_RDONLY);
	gnl = get_next_line(fd);
	while (gnl)
	{
		add_history(gnl);
		free(gnl);
		gnl = get_next_line(fd);
	}
	close(fd);
	return (0);
}

int	add_to_history(char *str, char path_to_history_file[50])
{
	int	fd;
	int	lenght;

	lenght = ft_strlen(str);
	if (lenght == 0)
		return (lenght);
	fd = open(path_to_history_file, O_CREAT | O_WRONLY | O_APPEND, 0777);
	if (fd < 0)
		exit(printf("Failed during history recording :(\n"));
	write(fd, str, lenght);
	write(fd, "\n", 1);
	add_history(str);
	close(fd);
	return (lenght);
}
