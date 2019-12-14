/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdelsie <vdelsie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/13 17:49:39 by vdelsie           #+#    #+#             */
/*   Updated: 2019/12/14 17:46:22 by vdelsie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <libft.h>
#include <ft_printf.h>

void	parse_format(t_vfpf *p)
{
	t_jumptable		handler_funct;
	const char		*fmt_start;

	fmt_start = p->fmt;
	if (p->fmt++ != '\0')
	{
		p->strt = p->fmt;
		parse_flags(p);
		parse_width(p);
		parse_precision(p, 0);
		parse_length(p);
		p->specifier = *p->fmt;
		if ((handler_funct = get_handler_funct(p->specifier)) == NULL)
			p->flags |= FMT_INVALID;
		else if (!(p->flags & FMT_INVALID))
		{
			handler_funct(p);
			p->fmt++;
		}
	}
	p->strt = (p->flags & FMT_INVALID) ? fmt_start : p->fmt;
}

int		ft_vfprintf(int fd, const char *format, va_list args)
{
	t_vfpf	p;

	if (ft_strequ("%", format))
		return (0);
	ft_bzero(&p, sizeof(p));
	p.fd = fd;
	va_copy(p.args, args);
	p.fmt = format;
	p.strt = p.fmt;
	while (*p.fmt)
	{
		if (*p.fmt != '%')
			p.fmt++;
		else
		{
			buff(&p, p.strt, p.fmt - p.strt);
			parse_format(&p);
			reset_printf(&p);
		}
	}
	buff(&p, p.strt, p.fmt - p.strt);
	flush_buffer(&p);
	va_end(p.args);
	return (p.len);
}

int		ft_printf(const char *format, ...)
{
	va_list	args;
	int		len;

	len = 0;
	if (format)
	{
		va_start(args, format);
		len = ft_vfprintf(1, format, args);
		va_end(args);
	}
	return (len);
}
