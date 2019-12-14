/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   int_handlers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdelsie <vdelsie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/13 17:48:48 by vdelsie           #+#    #+#             */
/*   Updated: 2019/12/13 17:49:01 by vdelsie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <ft_printf.h>

static intmax_t	get_signed_int_arg(t_vfpf *p)
{
	if (p->flags & L_FLAG || p->specifier == 'D')
		return ((intmax_t)va_arg(p->args, signed long));
	else if (p->flags & HH_FLAG)
		return ((intmax_t)(signed char)va_arg(p->args, signed int));
	else if (p->flags & H_FLAG)
		return ((intmax_t)(signed short)va_arg(p->args, signed int));
	else if (p->flags & LL_FLAG)
		return ((intmax_t)va_arg(p->args, signed long long));
	else if (p->flags & J_FLAG)
		return (va_arg(p->args, intmax_t));
	else if (p->flags & Z_FLAG)
		return ((intmax_t)va_arg(p->args, ssize_t));
	else if (p->flags & T_FLAG)
		return ((intmax_t)va_arg(p->args, ptrdiff_t));
	else
		return ((intmax_t)va_arg(p->args, signed int));
}

void			handle_int(t_vfpf *p)
{
	intmax_t	nbr;
	intmax_t	tmp;
	int			nbr_len;
	int			pf;

	pf = p->flags;
	p->base = 10;
	nbr = get_signed_int_arg(p);
	tmp = nbr;
	nbr_len = 0;
	p->neg = (nbr < 0) ? 1 : 0;
	while (tmp && ++nbr_len)
		tmp /= p->base;
	nbr_len += (pf & APOST_FLAG) ? ((nbr_len - 1) / 3) : 0;
	nbr_len += (nbr == 0 && !(pf & PRECI_OB_FLAG && p->precision == 0)) ? 1 : 0;
	if (nbr < 0 || pf & PLUS_FLAG || pf & SPACE_FLAG)
		p->width--;
	if (pf & PRECI_OB_FLAG)
		pf ^= ZERO_FLAG;
	handle_int_prepad(p, nbr_len, 1);
	pf_itoa_base(p, (uintmax_t)ABS(nbr), nbr_len);
	if (pf & WIDTH_OB_FLAG && (pf & DASH_FLAG))
		pad_width(p, MAX(p->precision, nbr_len));
}
