/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_char_handlers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdelsie <vdelsie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/13 17:43:38 by vdelsie           #+#    #+#             */
/*   Updated: 2019/12/14 17:39:34 by vdelsie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <ft_printf.h>
#include <wchar.h>

void			handle_wchar(t_vfpf *p)
{
	int		pf;
	wint_t	wc;

	pf = p->flags;
	wc = (wchar_t)va_arg(p->args, wint_t);
	if (pf & WIDTH_OB_FLAG && !(pf & DASH_FLAG))
		pad_width(p, 1);
	pf_putwchar(p, wc);
	if (pf & WIDTH_OB_FLAG && (pf & DASH_FLAG))
		pad_width(p, 1);
}

void			handle_char(t_vfpf *p)
{
	int		pf;
	char	c;

	pf = p->flags;
	if (pf & L_FLAG || p->specifier == 'C')
		handle_wchar(p);
	else
	{
		c = (char)va_arg(p->args, int);
		if (pf & WIDTH_OB_FLAG && !(pf & DASH_FLAG))
			pad_width(p, 1);
		buff(p, &c, 1);
		if (pf & WIDTH_OB_FLAG && (pf & DASH_FLAG))
			pad_width(p, 1);
	}
}

size_t			calc_precision(wchar_t *str, int precision, size_t new_prec)
{
	if (*str == '\0' || precision == 0)
		return (new_prec);
	else if (*str <= 0x007F)
		return (calc_precision(str + 1, precision - 1, new_prec + 1));
	else if (*str <= 0x07FF && precision >= 2)
		return (calc_precision(str + 1, precision - 2, new_prec + 2));
	else if (*str <= 0xFFFF && precision >= 3)
		return (calc_precision(str + 1, precision - 3, new_prec + 3));
	else if (*str <= 0x10FFFF && precision >= 4)
		return (calc_precision(str + 1, precision - 4, new_prec + 4));
	else
		return (new_prec);
}

void			handle_wstr(t_vfpf *p)
{
	int		pf;
	wchar_t	*wstr;
	int		wslen;

	pf = p->flags;
	if ((wstr = va_arg(p->args, wchar_t *)) == NULL)
		wstr = L"(null)";
	wslen = (int)pf_wstrlen(wstr);
	p->precision = (int)calc_precision(wstr, p->precision, 0);
	if (p->precision < 0)
		p->precision = wslen;
	p->precision = (p->precision > wslen) ? wslen : p->precision;
	wslen = (pf & PRECI_OB_FLAG) ? p->precision : wslen;
	if (pf & WIDTH_OB_FLAG && !(pf & DASH_FLAG))
		pad_width(p, wslen);
	pf_putwstr(p, wstr, wslen);
	if (pf & WIDTH_OB_FLAG && (pf & DASH_FLAG))
		pad_width(p, wslen);
}

void			handle_str(t_vfpf *p)
{
	int			pf;
	char		*str;
	int			slen;

	pf = p->flags;
	if (pf & L_FLAG || p->specifier == 'S')
		handle_wstr(p);
	else
	{
		if ((str = va_arg(p->args, char *)) == NULL)
			str = "(null)";
		slen = (int)ft_strlen(str);
		if (p->precision < 0)
			p->precision = slen;
		p->precision = (p->precision > slen) ? slen : p->precision;
		slen = (pf & PRECI_OB_FLAG) ? p->precision : slen;
		if (pf & WIDTH_OB_FLAG && !(pf & DASH_FLAG))
			pad_width(p, slen);
		buff(p, str, slen);
		if (pf & WIDTH_OB_FLAG && (pf & DASH_FLAG))
			pad_width(p, slen);
	}
}
