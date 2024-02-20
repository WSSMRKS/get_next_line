/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maweiss <maweiss@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 14:35:10 by maweiss           #+#    #+#             */
/*   Updated: 2024/02/20 16:50:15 by maweiss          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <dlfcn.h>

static char	*ft_fill_buffer(char *full_buff, int *nbr_nl)
{
	char	*tmp;
	char	*loc_nl;

	if (*nbr_nl < 0 || (size_t) * nbr_nl == ft_strlen(full_buff))
	{
		free (full_buff);
		return (NULL);
	}
	loc_nl = ft_strchr(full_buff, '\n');
	tmp = ft_substr(loc_nl + 1, 0, ft_strlen(loc_nl + 1));
	free(full_buff);
	return (tmp);
}

static char	*ft_ret_val(char *full_buff, int *nbr_nl)
{
	if (ft_strchr(full_buff, '\n'))
	{
		*nbr_nl = (int)(ft_strchr(full_buff, '\n') - full_buff + 1);
		return (ft_substr(full_buff, 0, *nbr_nl));
	}
	else
		return (ft_substr(full_buff, 0, strlen(full_buff)));
}

static char	*ft_read_if_no_nl(char *full_buff, int fd)
{
	int		len;
	int		res_read;
	char	*buffer;
	char	*to_free;

	res_read = 1;
	while (!ft_strchr(full_buff, '\n') && res_read > 0)
	{
		buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1));
		res_read = read(fd, buffer, BUFFER_SIZE);
		if (res_read > 0)
		{
			buffer[res_read] = 0;
			to_free = full_buff;
			full_buff = ft_strjoin(full_buff, buffer);
			if (to_free)
				free(to_free);
		}
		free(buffer);
	}
	len = ft_strlen(full_buff);
	if ((res_read <= 0 && len == 0) || res_read == -1)
	{
		free(full_buff);
		return (NULL);
	}
	else
		return (full_buff);
}

char	*get_next_line(int fd)
{
	static char	*stbuff[MAX_FD + 1];
	int			nbr_nl;
	char		*ret;

	if (fd < 0 || fd >= MAX_FD || BUFFER_SIZE < 1 || BUFFER_SIZE > INT_MAX)
		return (NULL);
	else
	{
		if (!stbuff[fd])
		{
			stbuff[fd] = malloc(sizeof(char) * 1);
			if (!stbuff[fd])
				return (NULL);
			stbuff[fd][0] = 0;
		}
	}
	stbuff[fd] = ft_read_if_no_nl(stbuff[fd], fd);
	if (!stbuff[fd])
		return (NULL);
	nbr_nl = -1;
	ret = ft_ret_val(stbuff[fd], &nbr_nl);
	stbuff[fd] = ft_fill_buffer(stbuff[fd], &nbr_nl);
	return (ret);
}

/*
Concept:
- Validate fd:
	fd < 0 Free everything, return NULL
	fd >= 0 valid start processing
- Analyze buffer:
	search for '\n' in existing buffer. (strchr \n)
		Found:	take string including nl sign (strchr \n).
				Put rest to buffer res (strlen),
				return
		Not Found: read_join repeat search for '\n'
		repeat
*/
