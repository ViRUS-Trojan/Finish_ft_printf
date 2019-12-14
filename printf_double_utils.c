/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printf_double_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdelsie <vdelsie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/13 17:46:48 by vdelsie           #+#    #+#             */
/*   Updated: 2019/12/13 17:47:18 by vdelsie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <ft_printf.h>
#define UPPER(x) ((x)=='X'||(x)=='F'||(x)=='E'||(x)=='G'||(x)=='A')
#define HEX(x) ((x)=='x'||(x)=='X'||(x)=='p'||(x)=='a'||(x)=='A')
#define DABS(x) (((x) < 0.0f) ? (-x) : (x))

uintmax_t		ft_pw(uintmax_t nb, uintmax_t power)
{
	uintmax_t i;

	if (power == 0)
		return (1);
	if (power % 2 == 0)
	{
		i = ft_pw(nb, power / 2);
		return (i * i);
	}
	else
		return (nb * ft_pw(nb, power - 1));
}

void			pf_ldtoa_int_base(t_vfpf *p, uintmax_t nbr, size_t int_len,
									char res[59])
{
	char		char_case;
	int			base;
	size_t		tmp_len;
	size_t		len_diff;

	base = p->base;
	char_case = ('a' - 10 - (('a' - 'A') * UPPER(p->specifier)));
	tmp_len = int_len;
	while (tmp_len--)
	{
		if (p->flags & LDTOA_FRAC && p->precision-- == 0)
			break ;
		len_diff = int_len - tmp_len;
		if (p->flags & APOST_FLAG && len_diff % 4 == 0 && len_diff > 0)
			res[tmp_len] = ',';
		else
		{
			res[tmp_len] = (nbr % base) + ((nbr % base < 10) ? '0' : char_case);
			nbr /= base;
		}
	}
}

void			pf_ldtoa_base(t_vfpf *p, long double nbr, size_t int_len,
							size_t tot_len)
{
	char		res[59];
	long double	frac;
	int			base;

	base = p->base;
	pf_ldtoa_int_base(p, (uintmax_t)DABS(nbr), int_len, res);
	p->flags |= LDTOA_FRAC;
	if (p->precision != 0)
	{
		if (p->flags & APOST_FLAG)
			p->flags ^= APOST_FLAG;
		p->precision++;
		frac = (DABS(nbr) - (uintmax_t)DABS(nbr)) * ft_pw(base, p->precision);
		frac = ((uintmax_t)DABS(frac) % base >= (uintmax_t)(base / 2)) ?
			(frac / base) + 1 : frac / base;
		pf_ldtoa_int_base(p, (uintmax_t)DABS(frac), tot_len, res);
	}
	if (p->precision != 0 || p->flags & HASH_FLAG)
		res[int_len] = '.';
	buff(p, res, tot_len);
}

static void		handle_double_prepend(t_vfpf *p)
{
	int	spec;

	spec = p->specifier;
	if (p->neg)
		buff(p, "-", 1);
	else if (p->flags & PLUS_FLAG)
		buff(p, "+", 1);
	else if (p->flags & SPACE_FLAG)
		buff(p, " ", 1);
	if (p->flags & HASH_FLAG && HEX(spec))
		(UPPER(spec)) ? buff(p, "0X", 2) : buff(p, "0x", 2);
}

void			handle_double_prepad(t_vfpf *p, int nbr_len)
{
	int		zp_len;
	int		wp_len;

	wp_len = (p->flags & PRECI_OB_FLAG) ? MAX(p->precision, nbr_len) : nbr_len;
	zp_len = (p->precision > nbr_len) ? p->precision - nbr_len : 0;
	if (p->flags & WIDTH_OB_FLAG && !(p->flags & DASH_FLAG))
	{
		if (p->flags & ZERO_FLAG)
			wp_len = p->width - wp_len;
		if (p->flags & PRECI_OB_FLAG)
			pad_width(p, zp_len + nbr_len);
		else if (!(p->flags & ZERO_FLAG))
			pad_width(p, wp_len);
	}
	handle_double_prepend(p);
	if (p->flags & PRECI_OB_FLAG)
		pad(p, zp_len, '0');
	else if (p->flags & WIDTH_OB_FLAG && !(p->flags & DASH_FLAG))
		if (p->flags & ZERO_FLAG)
			pad(p, wp_len, '0');
}
