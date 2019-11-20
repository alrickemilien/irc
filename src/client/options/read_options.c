/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_options_arguments.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemilien <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/15 14:55:41 by aemilien          #+#    #+#             */
/*   Updated: 2019/09/15 14:57:06 by aemilien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <client/irc.h>

static t_options_map	g_options_map[] = {
	{ "port", CLIENT_PORT, &read_port_option, },
	{ "host", CLIENT_HOST, &read_host_option, },
	{ "ipv6", CLIENT_IPV6, NULL, },
	{ "command", CLIENT_IPV6, &read_command_option, },
	{ "gui", CLIENT_GUI, NULL, },
	{ NULL, 0, NULL },
};

static int				handle_option(
		t_options *options,
		t_options_map **last,
		const char *name)
{
	size_t	j;
	size_t	len;

	len = strlen(name);
	j = 0;
	while (g_options_map[j].name != NULL)
	{
		if (!strcmp(name, g_options_map[j].name)
				&& len == strlen(g_options_map[j].name))
		{
			if (((int*)options)[g_options_map[j].offset] == 1)
			{
				return (fprintf(stderr,
				"for the -%s option: may only occur zero or one times!\n",
				g_options_map[j].name));
			}
			if (!g_options_map[j].waiting_for_value)
				((int*)options)[g_options_map[j].offset] = 1;
			else
				*last = &g_options_map[j];
			return (0);
		}
		j++;
	}
	return (fprintf(stderr, "Unknown command line argument '%s'\n", name));
}

void					read_one_options_argument(
	const char *arg,
	t_options *options,
	int *error)
{
	int						ret;
	static t_options_map	*waiting = NULL;

	ret = 0;
	if (waiting)
	{
		ret = waiting->waiting_for_value(options, arg);
		waiting = NULL;
	}
	else if (is_a_single_option(arg))
		ret = handle_option(options, &waiting, arg + 1);
	else if (is_a_multi_option(arg))
		ret = handle_option(options, &waiting, arg + 2);
	else
		ret = 1;
	if (ret != 0)
		*error = ret;
}

/*
** Read args by filling t_options structure with 0 or 1
** list is the list of files passed as parameter
*/

int						read_options(
	int ac, const char **av, t_options *options)
{
	int				i;
	int				error;

	memset(options, 0, sizeof(t_options));
	error = 0;
	i = 1;
	while (i < ac)
		read_one_options_argument(av[i++], options, &error);
	return (error);
}
