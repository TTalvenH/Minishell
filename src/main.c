/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaratzi <mkaratzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 03:59:16 by mkaratzi          #+#    #+#             */
/*   Updated: 2023/04/14 03:59:56 by mkaratzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(void)
{
	char	history_path[24];
	char	*line;

	line = NULL;
	get_history(history_path);
	printf("We got the path %s\n", history_path);
	while (1)
	{
		line = readline("minishell: ");
		if (!line)
			return (1);
		add_to_history(line, history_path);
		free(line);
	}
	return (0);
}
