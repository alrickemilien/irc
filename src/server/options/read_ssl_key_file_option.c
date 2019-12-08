/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_ssl_key_file_option.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemilien <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 15:00:36 by aemilien          #+#    #+#             */
/*   Updated: 2019/12/08 15:00:37 by aemilien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <server/irc.h>

int	read_ssl_key_file_option(t_options *options, const char *vsl)
{
	memcpy(options->ssl_key_file, vsl, strlen(vsl));
	if (access(options->ssl_key_file, R_OK) < 0)
		return (fprintf(stderr, "--ssl-key-file: cant stat file '%s'.\n", vsl));
	return (0);
}
