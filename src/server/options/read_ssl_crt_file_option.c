/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_ssl_crt_file_option.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemilien <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 14:59:46 by aemilien          #+#    #+#             */
/*   Updated: 2019/12/08 14:59:47 by aemilien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <server/irc.h>

int	read_ssl_crt_file_option(
		t_options *options, const char *vsl)
{
	memcpy(options->ssl_crt_file, vsl, strlen(vsl));
	if (access(options->ssl_crt_file, R_OK) < 0)
		return (fprintf(stderr, "--ssl-crt-file: cant stat file '%s'.\n", vsl));
	return (0);
}
