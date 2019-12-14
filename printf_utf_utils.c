/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printf_utf_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdelsie <vdelsie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/13 17:45:10 by vdelsie           #+#    #+#             */
/*   Updated: 2019/12/14 16:41:33 by vdelsie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <wchar.h>
#include <libft.h>
#include <ft_printf.h>

size_t	pf_wcharlen(wchar_t wc)
{
	if (wc <= 0x007F)
		return (1);
	else if (wc <= 0x07FF)
		return (2);
	else if (wc <= 0xFFFF)
		return (3);
	else if (wc <= 0x10FFFF)
		return (4);
	return (0);
}

size_t	pf_wstrlen(wchar_t *wstr)
{
	size_t	wlen;

	wlen = 0;
	while (*wstr != L'\0')
	{
		wlen += pf_wcharlen(*wstr);
		wstr++;
	}
	return (wlen);
}

void	pf_putchar(t_vfpf *p, char c)
{
	buff(p, &c, 1);
}

void	pf_putwchar(t_vfpf *p, wint_t wc)
{
	if (wc <= 0x007F)
		pf_putchar(p, wc);
	else if (wc <= 0x07FF)
	{
		pf_putchar(p, ((wc >> 6) | 0xC0));
		pf_putchar(p, ((wc & 0x3F) | 0x80));
	}
	else if (wc <= 0xFFFF)
	{
		pf_putchar(p, ((wc >> 12) | 0xE0));
		pf_putchar(p, (((wc >> 6) & 0x3F) | 0x80));
		pf_putchar(p, ((wc & 0x3F) | 0x80));
	}
	else if (wc <= 0x10FFFF)
	{
		pf_putchar(p, ((wc >> 18) | 0xF0));
		pf_putchar(p, (((wc >> 12) & 0x3F) | 0x80));
		pf_putchar(p, (((wc >> 6) & 0x3F) | 0x80));
		pf_putchar(p, ((wc & 0x3F) | 0x80));
	}
}

void	pf_putwstr(t_vfpf *p, wchar_t *wstr, size_t wlen)
{
	size_t	i;

	i = 0;
	while (*wstr && i < wlen)
	{
		if (*wstr <= 0x7F)
			i++;
		else if (*wstr <= 0x7FF)
			i += 2;
		else if (*wstr <= 0xFFFF)
			i += 3;
		else if (*wstr <= 0x10FFFF)
			i += 4;
		if (i <= wlen)
			pf_putwchar(p, *wstr++);
	}
}
