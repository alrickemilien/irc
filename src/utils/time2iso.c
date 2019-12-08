/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time2iso.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemilien <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 13:18:41 by aemilien          #+#    #+#             */
/*   Updated: 2019/12/08 13:18:53 by aemilien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <time.h>
#include <irc.h>

int	time2iso(char *str)
{
	time_t		timer;
	struct tm	*tm_info;

	time(&timer);
	tm_info = localtime(&timer);
	if (strftime(str, ISOTIMESTRSIZE, "%Y-%m-%d %H:%M:%S%z", tm_info) == 0)
		return (-1);
	return (0);
}
