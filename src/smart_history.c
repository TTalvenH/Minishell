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

int	get_history_path(char path_to_history_file[21])
{
	int	check;
	int	i;
	int	pid;

	i = 0;
	check = 0;
	pid = getpid();
	ft_strlcpy(path_to_history_file, "./.minishell_history", 21);
	return (0);
}

int	get_history(char path_to_history_file[21])
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

int	add_to_history(char *str, char path_to_history_file[21])
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
