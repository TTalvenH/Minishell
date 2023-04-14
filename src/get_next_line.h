/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gNL.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaratzi <mkaratzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 12:20:32 by mkaratzi          #+#    #+#             */
/*   Updated: 2022/11/22 11:25:17 by mkaratzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# include <stdlib.h>
# include <unistd.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

char	*get_next_line(int fd);
int		ft_strlenmod(char *string, int instruction);
int		join(char **dest, char *src, char *src2, int check);
int		ft_checklast(char *string);

#endif