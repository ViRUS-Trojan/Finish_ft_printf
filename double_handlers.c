/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   double_handlers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdelsie <vdelsie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/13 17:50:10 by vdelsie           #+#    #+#             */
/*   Updated: 2019/12/13 17:50:57 by vdelsie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_printf.h>
#define DABS(x) (((x) < 0.0f) ? (-x) : (x))
#define DOUBLE_UPPER(x) ((x)=='F'||(x)=='E'||(x)=='G'||(x)=='A')

static int			ld_isnan(long double d)
{
	union u_union	u;

	u.d = (double)(d);
	return (u.l == 0x7FF8000000000000ll || u.l == 0xFFF8000000000000ll);
}

static int			ld_isinf(long double d)
{
	union u_union	u;

	u.d = (double)(d);
	if (u.l == 0x7FF0000000000000ll)
		return (1);
	else if (u.l == 0xFFF0000000000000ll)
		return (-1);
	else
		return (0);
}

long double			get_ld_arg(t_vfpf *p, size_t *len, int base)
{
	long double		nbr;
	uintmax_t		tmp;

	*len = 0;
	if (p->flags & FLOAT_L_FLAG)
		nbr = va_arg(p->args, long double);
	else
		nbr = (long double)va_arg(p->args, double);
	tmp = (uintmax_t)DABS(nbr);
	if (tmp == 0)
		*len += 1;
	while (tmp && ++(*len))
		tmp /= base;
	return (nbr);
}

static void			handle_invalid(t_vfpf *p, long double nbr, int inf_t)
{
	int len;

	len = ((inf_t == -1 || p->flags & PLUS_FLAG) && !ld_isnan(nbr)) ? 4 : 3;
	(p->flags & ZERO_FLAG) ? p->flags ^= ZERO_FLAG : 0;
	handle_int_prepad(p, len, 0);
	if (ld_isnan(nbr))
	{
		if (DOUBLE_UPPER(p->specifier))
			buff(p, "NAN", 3);
		else
			buff(p, "nan", 3);
	}
	else if (inf_t)
	{
		if (inf_t == -1)
			buff(p, "-", 1);
		else if (p->flags & PLUS_FLAG)
			buff(p, "+", 1);
		if (DOUBLE_UPPER(p->specifier))
			buff(p, "INF", 3);
		else
			buff(p, "inf", 3);
	}
	if (p->flags & WIDTH_OB_FLAG && p->flags & DASH_FLAG)
		pad_width(p, len);
}

void				handle_double(t_vfpf *p)
{
	long double		nbr;
	size_t			int_len;
	size_t			tot_len;

	p->base = 10;
	nbr = get_ld_arg(p, &int_len, p->base);
	if (ld_isinf(nbr) || ld_isnan(nbr))
		handle_invalid(p, nbr, ld_isinf(nbr));
	else
	{
		if (!(p->flags & PRECI_OB_FLAG) || p->precision < 0)
			p->precision = 6;
		tot_len = (p->precision != 0 || p->flags & HASH_FLAG) ? 1 : 0;
		p->neg = (nbr < 0.0) ? 1 : 0;
		int_len += (p->flags & APOST_FLAG) ? ((int_len - 1) / 3) : 0;
		tot_len += int_len + p->precision;
		if (nbr < 0.0 || p->flags & PLUS_FLAG || p->flags & SPACE_FLAG)
			p->width--;
		handle_double_prepad(p, tot_len);
		pf_ldtoa_base(p, nbr, int_len, tot_len);
		if (p->flags & WIDTH_OB_FLAG && p->flags & DASH_FLAG)
			pad_width(p, tot_len);
	}
}
