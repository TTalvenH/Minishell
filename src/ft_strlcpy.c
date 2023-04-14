/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaratzi <mkaratzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 09:53:00 by mkaratzi          #+#    #+#             */
/*   Updated: 2023/04/14 02:50:43 by mkaratzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	src_length;

	src_length = ft_strlen(src);
	if (size == 0)
		return (src_length);
	while (--size > 0)
	{
		if (!*src)
			break ;
		*(dst++) = *(src++);
	}
	*dst = '\0';
	return (src_length);
}

size_t	ft_strlen(const char *s)
{
	int	length;

	length = 0;
	while (*(s++))
		length++;
	return (length);
}
