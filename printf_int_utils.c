/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printf_int_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdelsie <vdelsie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/13 17:46:06 by vdelsie           #+#    #+#             */
/*   Updated: 2019/12/13 17:46:09 by vdelsie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <ft_printf.h>
#define UPPER(x) ((x)=='X'||(x)=='F'||(x)=='E'||(x)=='G'||(x)=='A')
#define HEX(x) ((x)=='x'||(x)=='X'||(x)=='p'||(x)=='a'||(x)=='A')

void			pf_itoa_base(t_vfpf *p, uintmax_t nbr, int nbr_len)
{
	char	res[27];
	int		tmp_len;
	int		len_diff;
	int		base;
	char	char_case;

	base = p->base;
	char_case = ('a' - 10 - (('a' - 'A') * UPPER(p->specifier)));
	tmp_len = nbr_len;
	while (tmp_len--)
	{
		len_diff = nbr_len - tmp_len;
		if (p->flags & APOST_FLAG && len_diff % 4 == 0 && len_diff > 0)
			res[tmp_len] = ',';
		else
		{
			res[tmp_len] = (nbr % base) + ((nbr % base < 10) ? '0' : char_case);
			nbr /= base;
		}
	}
	buff(p, res, nbr_len);
}

static void		handle_prepend(t_vfpf *p, int signed_int)
{
	int	spec;

	spec = p->specifier;
	if (p->neg)
		buff(p, "-", 1);
	else if (p->flags & PLUS_FLAG && signed_int)
		buff(p, "+", 1);
	else if (p->flags & SPACE_FLAG && signed_int)
		buff(p, " ", 1);
	if (p->flags & HASH_FLAG && HEX(spec) && ((p->hex_int != 0) || spec == 'p'))
		(UPPER(spec)) ? buff(p, "0X", 2) : buff(p, "0x", 2);
}

void			handle_int_prepad(t_vfpf *p, int nbr_len, int signed_int)
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
	handle_prepend(p, signed_int);
	if (p->flags & PRECI_OB_FLAG)
		pad(p, zp_len, '0');
	else if (p->flags & WIDTH_OB_FLAG && !(p->flags & DASH_FLAG))
		if (p->flags & ZERO_FLAG)
			pad(p, wp_len, '0');
}
