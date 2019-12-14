/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printf_gen_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdelsie <vdelsie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/13 17:46:19 by vdelsie           #+#    #+#             */
/*   Updated: 2019/12/13 20:03:36 by vdelsie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <libft.h>
#include <ft_printf.h>

void	flush_buffer(t_vfpf *p)
{
	write(p->fd, p->buf, p->buf_len);
	p->buf_len = 0;
}

void	buff(t_vfpf *p, const void *s, size_t nbyte)
{
	unsigned char	*src;
	size_t			remain;

	src = (unsigned char *)s;
	while (p->buf_len + nbyte > BUFF_SIZE)
	{
		remain = BUFF_SIZE - p->buf_len;
		while (remain-- && nbyte--)
		{
			p->buf[p->buf_len++] = *src++;
			p->len++;
		}
		flush_buffer(p);
	}
	while (nbyte--)
	{
		p->buf[p->buf_len++] = *src++;
		p->len++;
	}
}

void	pad(t_vfpf *p, int pad_len, char pad_char)
{
	char	*pad_str;

	pad_str = NULL;
	if (pad_len > 0)
	{
		if ((pad_str = (char *)malloc(pad_len + 1)) != NULL)
		{
			pad_str[pad_len] = '\0';
			ft_memset(pad_str, pad_char, (size_t)pad_len);
			buff(p, pad_str, pad_len);
			free(pad_str);
		}
	}
}

void	pad_width(t_vfpf *p, int arg_width)
{
	int final_pad_width;

	final_pad_width = p->width - arg_width;
	pad(p, final_pad_width, ' ');
}

void	reset_printf(t_vfpf *p)
{
	p->flags = 0;
	p->width = 0;
	p->precision = 0;
	p->base = 0;
	p->neg = 0;
	p->hex_int = 0;
	p->specifier = '\0';
}
