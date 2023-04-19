/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaratzi <mkaratzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/27 12:56:43 by ttalvenh          #+#    #+#             */
/*   Updated: 2023/04/16 06:13:45 by mkaratzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	if (!n)
		return (0);
	while ((n > 1 && *s1 == *s2) && (*s1 || *s2))
	{
		s1++;
		s2++;
		n--;
	}
	return ((unsigned const char)*s1 - (unsigned const char)*s2);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	while ((*s1 == *s2) && (*s1 || *s2))
	{
		s1++;
		s2++;
	}
	return ((unsigned const char)*s1 - (unsigned const char)*s2);
}