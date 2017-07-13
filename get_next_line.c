/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-meur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/08 11:09:42 by tle-meur          #+#    #+#             */
/*   Updated: 2015/12/10 14:26:59 by tle-meur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static int	size_buf(char *buf)
{
	int		i;

	i = 0;
	while (i < BUFF_SIZE && buf[i] != '\n' && buf[i])
		i++;
	return (i);
}

static int	copy_buf(char *buf, char **line, int is_reading)
{
	char	endbuf;
	int		buffsize;
	int		linesize;

	buffsize = size_buf(buf);
	linesize = ft_strlen(*line);
	if (!(*line = ft_memrealloc(*line, linesize, linesize + buffsize + 1)))
		return (-1);
	if (!ft_memcpy(*line + linesize, buf, buffsize))
		return (-1);
	if (buffsize != BUFF_SIZE)
	{
		endbuf = buf[buffsize];
		if (!ft_memmove(buf, buf + buffsize + 1, BUFF_SIZE - buffsize - 1)
			|| !ft_memset(buf + BUFF_SIZE - buffsize - 1, 0, buffsize + 1))
			return (-1);
		return ((is_reading || *buf || endbuf == '\n') ? 1 : 0);
	}
	return ((!ft_memset(buf, 0, buffsize)) ? -1 : 0);
}

int			get_next_line(int const fd, char **line)
{
	int			ret;
	char		*tmp;
	static char	buf[MAX_FD][BUFF_SIZE];

	tmp = NULL;
	ft_memdel((void **)line);
	if (fd < 0 || fd >= MAX_FD || !line)
		return (-1);
	if (buf[fd][0])
		if ((ret = copy_buf(buf[fd], &tmp, 0)))
			return ((*line = tmp) ? ret : ret);
	while ((ret = read(fd, buf[fd], BUFF_SIZE)) > 0)
		if ((ret = copy_buf(buf[fd], &tmp, 1)))
			return ((*line = tmp) ? ret : ret);
	return (((*line = tmp) && ret != -1 && *tmp) ? 1 : ret);
}
