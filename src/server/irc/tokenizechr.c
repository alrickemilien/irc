/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizechr.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemilien <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 16:18:03 by aemilien          #+#    #+#             */
/*   Updated: 2019/12/08 16:18:03 by aemilien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <irc.h>
#include <server/irc.h>

size_t	tokenizechr(
		char *str, t_token *tokens, size_t len, int c)
{
	size_t	i;
	size_t	count;
	char	*end;

	memset(tokens, 0, sizeof(t_token) * len);
	end = strstr(str, "\x0D\x0A");
	count = 0;
	if (end == (void *)0)
		end = str + strlen(str);
	i = 0;
	while (str[i] == 0x20)
		i++;
	if (!str[i])
		return (0);
	while (str[i] && str[i] != 0x20 && str + i < end && count < len)
	{
		while (str[i] == c)
			i++;
		if (str[i] == 0 || str[i] == 0x20)
			break ;
		tokens[count].addr = str + i;
		while (str[i] != c && str[i] && str[i] != 0x20 && str + i < end)
			i++;
		tokens[count].len = (size_t)(&str[i] - tokens[count].addr);
		count++;
	}
	return (count);
}
