/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttalvenh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/24 13:07:14 by ttalvenh          #+#    #+#             */
/*   Updated: 2022/11/24 13:28:47 by ttalvenh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	vec_init_or_grow(t_vec *vec, size_t new_size)
{
	char	*tmp;

	if (vec->u == 0)
	{
		if (vec->str)
			free(vec->str);
		vec->str = malloc(sizeof(char) * (new_size + 1));
		if (!vec->str)
			return (-1);
		vec->str[new_size] = '\0';
		return (0);
	}
	else
	{
		tmp = malloc (sizeof(char) * new_size + 1);
		if (!tmp)
			return (-1);
		tmp[new_size] = '\0';
		ft_gnl_strlcpy(tmp, vec->str, vec->u + 1);
		free(vec->str);
		vec->str = NULL;
		vec->str = tmp;
	}
	return (0);
}

char	*vec_free(t_vec *vec)
{
	char	*rtn;

	rtn = ft_gnl_strndup(vec->str, vec->i);
	free (vec->str);
	vec->str = NULL;
	vec->u = 0;
	return (rtn);
}

char	*vec_cut(t_vec *vec)
{
	char	*tmp;
	char	*rtn;

	rtn = ft_gnl_strndup(vec->str, vec->i + 1);
	tmp = malloc (sizeof(char) * (vec->u - vec->i));
	if (!tmp)
		return (NULL);
	tmp[vec->u - (vec->i + 1)] = '\0';
	ft_gnl_strlcpy(tmp, &vec->str[vec->i + 1], vec->u - vec->i + 1);
	free(vec->str);
	vec->str = NULL;
	vec->u = vec->u - (vec->i + 1);
	vec->i = 0;
	vec->str = tmp;
	return (rtn);
}

char	*copy(char *buff, int r)
{
	static t_vec	vec = {NULL, 0, 0};
	char			*rtn;

	rtn = NULL;
	if (r)
	{
		if (vec_init_or_grow(&vec, vec.u + r) < 0)
			return (NULL);
		vec.u = vec.u + r;
		ft_gnl_strlcat(vec.str, buff, vec.u + 1, vec.u - r);
	}
	while (vec.i < vec.u)
	{
		if (vec.str[vec.i] == '\n' || (vec.i + 1 == vec.u && r < BUFF_SIZE))
			return (vec_cut(&vec));
		vec.i++;
	}
	if (!r)
	{
		if (vec.str)
			rtn = vec_free(&vec);
		vec.i = 0;
	}
	return (rtn);
}

char	*get_next_line(int fd)
{
	char	*buff;
	char	*rtn;
	int		r;

	r = 1;
	rtn = NULL;
	buff = malloc(sizeof(char) * (BUFF_SIZE + 1));
	if (!buff)
		return (NULL);
	buff[BUFF_SIZE] = '\0';
	while (!rtn && r)
	{
		r = read(fd, buff, BUFF_SIZE);
		if (r < 0)
		{
			free (buff);
			buff = NULL;
			return (NULL);
		}
		rtn = copy(buff, r);
	}
	free(buff);
	return (rtn);
}
