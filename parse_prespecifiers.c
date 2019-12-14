/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_prespecifiers.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdelsie <vdelsie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/13 17:48:05 by vdelsie           #+#    #+#             */
/*   Updated: 2019/12/13 17:48:27 by vdelsie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <ft_printf.h>
#define IS_FLAG(x) ((x)=='#'||(x)=='0'||(x)=='-'||(x)=='+'||(x)==' '||(x)=='\'')
#define IS_LEN(y) ((y)=='h'||(y)=='l'||(y)=='j'||(y)=='z'||(y)=='t'||(y)=='L')
#define IS_SLEN(z) ((z)=='j'||(z)=='z'||(z)=='t'||(z)=='L')

void			parse_flags(t_vfpf *p)
{
	if (IS_FLAG(*p->fmt))
	{
		if (*p->fmt == '#')
			p->flags |= HASH_FLAG;
		else if (*p->fmt == '0')
			p->flags |= ZERO_FLAG;
		else if (*p->fmt == '-')
			p->flags |= DASH_FLAG;
		else if (*p->fmt == '+')
			p->flags |= PLUS_FLAG;
		else if (*p->fmt == ' ')
			p->flags |= SPACE_FLAG;
		else if (*p->fmt == '\'')
			p->flags |= APOST_FLAG;
		p->fmt++;
		parse_flags(p);
	}
}

void			parse_width(t_vfpf *p)
{
	int tentative_width;

	if (!(p->flags & FMT_INVALID) && (*p->fmt == '*' || ft_isdigit(*p->fmt)))
	{
		p->flags |= (p->flags & WIDTH_OB_FLAG) ? FMT_INVALID : 0;
		if (!(p->flags & FMT_INVALID) && *p->fmt == '*')
		{
			p->fmt++;
			tentative_width = va_arg(p->args, int);
			p->flags |= (tentative_width < 0) ? DASH_FLAG : 0;
			p->width = ABS(tentative_width);
			p->flags |= WIDTH_OB_FLAG;
		}
		if (!(p->flags & FMT_INVALID) && ft_isdigit(*p->fmt))
		{
			p->flags |= (p->flags & WIDTH_OB_FLAG) ? FMT_INVALID : 0;
			while (!(p->flags & FMT_INVALID) && ft_isdigit(*p->fmt))
				p->width = p->width * 10 + (*(p->fmt++) - '0');
			p->flags |= WIDTH_OB_FLAG;
		}
		parse_width(p);
	}
}

void			parse_precision(t_vfpf *p, int recurse_level)
{
	if (!(p->flags & FMT_INVALID) && (*p->fmt == '.' || recurse_level > 0))
	{
		p->fmt = (*p->fmt == '.') ? p->fmt + 1 : p->fmt;
		if (*p->fmt == '*' || ft_isdigit(*p->fmt))
		{
			p->flags |= (p->flags & PRECI_OB_FLAG) ? FMT_INVALID : 0;
			if (!(p->flags & FMT_INVALID) && *p->fmt == '*')
			{
				p->fmt++;
				p->precision = va_arg(p->args, int);
				p->flags |= PRECI_OB_FLAG;
			}
			if (!(p->flags & FMT_INVALID) && ft_isdigit(*p->fmt))
			{
				p->flags |= (p->flags & PRECI_OB_FLAG) ? FMT_INVALID : 0;
				while (!(p->flags & FMT_INVALID) && ft_isdigit(*p->fmt))
					p->precision = p->precision * 10 + (*(p->fmt++) - '0');
				p->flags |= PRECI_OB_FLAG;
			}
			parse_precision(p, recurse_level + 1);
		}
		p->flags |= PRECI_OB_FLAG;
	}
}

static void		parse_long_length(t_vfpf *p, char c, int s_flag, int d_flag)
{
	if (*p->fmt == c)
	{
		if (p->flags & LEN_OB_FLAG)
		{
			if ((p->flags & s_flag) && !(p->flags & d_flag))
			{
				p->flags ^= s_flag;
				p->flags |= d_flag;
			}
			else
				p->flags |= FMT_INVALID;
		}
		else
		{
			p->flags |= s_flag;
			p->flags |= LEN_OB_FLAG;
		}
	}
}

void			parse_length(t_vfpf *p)
{
	if (!(p->flags & FMT_INVALID) && IS_LEN(*p->fmt))
	{
		parse_long_length(p, 'h', H_FLAG, HH_FLAG);
		parse_long_length(p, 'l', L_FLAG, LL_FLAG);
		if (IS_SLEN(*p->fmt))
		{
			if (!(p->flags & LEN_OB_FLAG))
			{
				p->flags |= (*p->fmt == 'j') ? J_FLAG : 0;
				p->flags |= (*p->fmt == 'z') ? Z_FLAG : 0;
				p->flags |= (*p->fmt == 't') ? T_FLAG : 0;
				p->flags |= (*p->fmt == 'L') ? FLOAT_L_FLAG : 0;
				p->flags |= LEN_OB_FLAG;
			}
			else
				p->flags |= FMT_INVALID;
		}
		p->fmt++;
		parse_length(p);
	}
	else if (*p->fmt == '.')
		p->flags |= FMT_INVALID;
}
