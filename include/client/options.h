/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   options.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemilien <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/14 15:04:33 by aemilien          #+#    #+#             */
/*   Updated: 2019/12/14 15:04:34 by aemilien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_OPTIONS_H
# define CLIENT_OPTIONS_H

# include <stdint.h>
# include <string.h>
# include <stdbool.h>
# include <stdio.h>

/*
** All parameter
*/

enum {
	CLIENT_PORT = 0U,
	CLIENT_HOST,
	CLIENT_IPV6,
	CLIENT_GUI,
	CLIENT_SSL,
	OPTIONS_NUMBER,
};

/*
** MAximum value of <string> allowed for --opt=<string> or --opt <string>
*/

# define MAX_ALLOWED_VALUE_SIZE 215
# define MAX_COMMAND_SIZE 2048

/*
** Structure filled with options passed to program
** The handled options are currently -l, -R, -a, -r et -t
** None ls option used in the code :
** print_dir_name : print the dir name in some case at top of list
** At the end stor tty infos
** !! WARNING !!
** Options in struct s_options must be in the same order
** that the enum
*/

# pragma pack(push, 1)

typedef struct	s_options {
	int			port;
	int			l_host;
	int			ipv6;
	int			gui;
	int			ssl;
	char		host[MAX_ALLOWED_VALUE_SIZE];
	char		str_port[9];
}				t_options;

# pragma pack(pop)

/*
** Map used to handle each option :
** name : Char* value
** offset: offset in the structure s_options
** waiting_for_value: is option waiting for argument
*/

typedef struct	s_options_map {
	char		*name;
	int			offset;
	int			(*waiting_for_value)(
				t_options *options,
				const char *value);
}				t_options_map;

typedef struct	s_args_map {
	int			offset;
	int			(*waiting_for_value)(
				t_options *options,
				const char *value);
}				t_args_map;

/*
** Options utils
*/

bool			is_a_multi_option(const char *name);
bool			is_a_single_option(const char *name);
bool			is_an_option(const char *name);
bool			is_a_waiting_value_option(const char *name);
int				read_options(
		int ac, const char **av, t_options *opt);

/*
** Options with specifc values to handle
*/

int				read_port_option(
	t_options *options, const char *value);
int				read_host_option(
	t_options *options, const char *value);
#endif
