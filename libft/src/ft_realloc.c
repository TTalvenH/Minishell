/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaratzi <mkaratzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 18:14:49 by mkaratzi          #+#    #+#             */
/*   Updated: 2023/06/12 18:15:16 by mkaratzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_realloc(char *str, int new_size)
{
	char	*new_str;

	new_str = malloc(sizeof(char) * new_size + 1);
	if (!new_str)
		return (NULL);
	ft_strlcpy(new_str, str, new_size - 1);
	free(str);
	str = NULL;
	return (new_str);
}
