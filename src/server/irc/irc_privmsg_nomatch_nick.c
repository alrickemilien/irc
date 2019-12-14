/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_privmsg_nomatch_nick.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemilien <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/14 11:57:40 by aemilien          #+#    #+#             */
/*   Updated: 2019/12/14 11:57:46 by aemilien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <server/irc.h>

void		irc_privmsg_nomatch_nick(
		t_env *e,
		int cs,
		t_token *subtokens,
		size_t subtoken_count)
{
	size_t j;

	j = 0;
	while (j < subtoken_count)
	{
		if (subtokens[j].addr != NULL && subtokens[j].addr[0] != '&' &&
				subtokens[j].addr[0] != '#')
			irc_err(e, cs, ERR_NOSUCHNICK, subtokens[j].addr);
		j++;
	}
}
