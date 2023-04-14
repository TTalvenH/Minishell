/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaratzi <mkaratzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 23:16:57 by mkaratzi          #+#    #+#             */
/*   Updated: 2023/04/14 04:01:51 by mkaratzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int		ft_strlenmod(char *string, int instruction);
int		join(char **dest, char *src, char *src2, int check);
int		ft_checklast(char *string);

int	join(char **dest, char *src, char *src2, int check)
{
	long long	a;
	long long	c;
	long long	b;
	char		*final;

	final = NULL;
	a = ft_strlenmod(src, check);
	c = -1;
	b = ft_strlenmod(src2, 0);
	if (a + b > 0)
	{
		final = (char *)malloc(sizeof(char) * (a + b + 1));
	}
	while (final != NULL && ++c < a + b)
	{
		if (src != NULL && c < a)
			final[c] = src[c];
		else if (src2 != NULL && c < a + b)
			final[c] = src2[c - a];
		final[a + b] = '\0';
	}
	if (dest[0] != NULL)
		free(dest[0]);
	dest[0] = final;
	return (a + b);
}

int	ft_strlenmod(char *string, int instruction)
{
	int	length;

	length = 0;
	if (string == NULL)
		return (length);
	while (string[length] != '\0')
	{
		if (string[length++] == '\n' && instruction == -1)
			break ;
	}
	return (length);
}

int	ft_checklast(char *string)
{
	int	length;

	length = 0;
	if (string == NULL)
		return (length);
	while (string[length] != '\0')
		if (string[length++] == '\n')
			return (1);
	return (0);
}
