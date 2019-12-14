/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemilien <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/14 12:39:14 by aemilien          #+#    #+#             */
/*   Updated: 2019/12/14 12:39:16 by aemilien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <client/irc.h>

size_t	tokenize(const char *str, t_token *tokens, size_t len)
{
	size_t	i;
	size_t	count;

	memset(tokens, 0, sizeof(t_token) * len);
	i = 0;
	count = 0;
	while (str[i] && str[i] != 0x0A && str[i] != 0x0D && count < len)
	{
		while (str[i] == 0x20 && str[i])
			i++;
		if (str[i] == 0 || str[i] == 0x0A)
			break ;
		tokens[count].addr = str + i;
		while (str[i] != 0x20 && str[i] != 0x0A && str[i] != 0x0D && str[i])
		{
			tokens[count].len++;
			i++;
		}
		count++;
	}
	return (count);
}
