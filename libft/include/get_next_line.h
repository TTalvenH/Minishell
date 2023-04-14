/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttalvenh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/24 13:07:26 by ttalvenh          #+#    #+#             */
/*   Updated: 2022/11/24 13:29:33 by ttalvenh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# define BUFF_SIZE 42

# include <stdlib.h>
# include <unistd.h>

typedef struct s_vec
{
	char	*str;
	size_t	u;
	size_t	i;
}t_vec;

char	*get_next_line(int fd);
void	ft_gnl_strlcpy(char *dst, const char *src, size_t dstsize);
char	*ft_gnl_strndup(const char *s1, size_t n);
void	ft_gnl_strlcat(char *dst, const char *src, size_t dstsize, size_t dstl);

#endif
