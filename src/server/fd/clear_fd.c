/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clear_fd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemilien <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 14:51:00 by aemilien          #+#    #+#             */
/*   Updated: 2019/12/08 14:51:01 by aemilien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <server/irc.h>
#include <server/ssl.h>

void	clear_fd(t_fd *fd)
{
	fd->registered = 0;
	fd->away = 0;
	fd->channel = 0;
	fd->type = FD_FREE;
	fd->read = NULL;
	fd->write = NULL;
	cbuffer_reset(&fd->buf_read);
	cbuffer_reset(&fd->buf_write);
	if (fd->ssl)
		SSL_free(fd->ssl);
}
