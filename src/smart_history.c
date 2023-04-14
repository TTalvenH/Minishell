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

int	get_history_path(char path_to_history_file[24])
{
	int	check;
	int	i;

	i = 0;
	check = 0;
	ft_strlcpy(path_to_history_file, "/tmp/.minishell_history", 24);
	while (path_to_history_file[i])
	{
		check += path_to_history_file[i];
		i++;
	}
	if (check != 2323)
		return (1);
	return (0);
}

int	get_history(char path_to_history_file[24])
{
	char	*gnl;
	int		fd;

	if (get_history_path(path_to_history_file))
		exit(printf("Failed to initialise history path string :(\n"));
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

int	add_to_history(char *str, char path_to_history_file[24])
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
