/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_specifier.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdelsie <vdelsie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/13 17:47:31 by vdelsie           #+#    #+#             */
/*   Updated: 2019/12/13 17:47:47 by vdelsie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <ft_printf.h>

static void		setup_jump_table(t_jumptable j_table[128])
{
	j_table['%'] = &handle_escape;
	j_table['d'] = &handle_int;
	j_table['D'] = &handle_int;
	j_table['i'] = &handle_int;
	j_table['o'] = &handle_unsigned_int_oct;
	j_table['O'] = &handle_unsigned_int_oct;
	j_table['u'] = &handle_unsigned_int_dec;
	j_table['U'] = &handle_unsigned_int_dec;
	j_table['x'] = &handle_unsigned_int_hex;
	j_table['X'] = &handle_unsigned_int_hex;
	j_table['f'] = &handle_double;
	j_table['F'] = &handle_double;
	j_table['e'] = &handle_double_sci;
	j_table['E'] = &handle_double_sci;
	j_table['g'] = &handle_double_shortest;
	j_table['G'] = &handle_double_shortest;
	j_table['a'] = &handle_double_hex;
	j_table['A'] = &handle_double_hex;
	j_table['c'] = &handle_char;
	j_table['C'] = &handle_char;
	j_table['s'] = &handle_str;
	j_table['S'] = &handle_str;
	j_table['p'] = &handle_unsigned_int_hex;
	j_table['n'] = &handle_n;
}

t_jumptable		get_handler_funct(char c)
{
	static t_jumptable	specifier_jt[128] = {NULL};

	if (specifier_jt['%'] == NULL)
		setup_jump_table(specifier_jt);
	if (ft_isascii(c))
		return (specifier_jt[(int)c]);
	else
		return (NULL);
}
