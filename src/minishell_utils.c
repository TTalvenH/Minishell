#include "minishell.h"
#include "libft.h"

void	print_2d_array(char **arr)
{
	while(*arr)
	{
		ft_printf("%s\n", *arr);
		arr++;
	}
}
