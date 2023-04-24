#include "libft.h"

char	*ft_realloc(char *str, int new_size)
{
	char *new_str;

	new_str = malloc(sizeof(char) * new_size + 1);
	if (!new_str)
		return (NULL);
	ft_strlcpy(new_str, str, new_size - 1);
	free(str);
	str = NULL;
	return (new_str);
}