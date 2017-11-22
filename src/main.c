/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rschramm <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/30 15:58:34 by rschramm          #+#    #+#             */
/*   Updated: 2017/11/20 09:12:47 by rschramm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/termtest.h"

typedef struct		s_terminal
{
	struct termios	term;
	struct winsize	sz;
	char			*name;
	char			char_buff[5];
	char			line_buff[4096];
}					t_terminal;

typedef struct		s_input
{
	char			char_buff[5];
	char			line_buff[4096];
}					t_input;

int		constructor(t_terminal *config)
{
	if ((tgetent(NULL, getenv("TERM")) < 1))
		return (0);
	if ((config->name == getenv("xterm-256color")) == 0)
		ft_dprintf(2, "Opps, problem with terminal name\n");
	tcgetattr(0, &config->term);
	config->term.c_lflag &= ~(ICANON | ECHO);
	config->term.c_cc[VMIN] = 1;
	config->term.c_cc[VTIME] = 0;
	tcsetattr(0, TCSANOW, &config->term);
	return (1);
}

void	build_line(t_input *data)
{
	ft_printf("inside build_line: %c\n", data->char_buff[0]);
	strcat(data->line_buff, data->char_buff);
	ft_printf("Voila, your line: %s\n", data->line_buff);
}

int		main(void)
{
	t_terminal	config;
	t_input		data;

	if (!(constructor(&config)))
		return (0);
	while ((read(0, &data.char_buff, 5)) && data.char_buff[0] != 10)
	{
		ft_printf("You just entered: %c\n", data.char_buff[0]);
		if (data.char_buff[0] == '\E')
			break;
		if (ft_isprint(data.char_buff[0]))
			build_line(&data);
	}
	return (0);
}
