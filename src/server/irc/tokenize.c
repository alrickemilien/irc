/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemilien <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 16:15:42 by aemilien          #+#    #+#             */
/*   Updated: 2019/12/08 16:16:15 by aemilien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <server/irc.h>

size_t	tokenize(char *str, t_token *tokens, size_t len)
{
	size_t	i;
	size_t	count;
	char	*end;

	memset(tokens, 0, sizeof(t_token) * len);
	end = strstr(str, "\x0D\x0A");
	if (end == (void*)0)
		end = str + strlen(str);
	i = 0;
	count = 0;
	while (str[i] && str + i < end && count < len)
	{
		while (str[i] == 0x20 && str[i])
			i++;
		if (str[i] == 0)
			break ;
		tokens[count].addr = str + i;
		while (str[i] != 0x20 && str[i] && str + i < end)
			i++;
		tokens[count].len = (size_t)(&str[i] - tokens[count].addr);
		count++;
	}
	return (i);
}
