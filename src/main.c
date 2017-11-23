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

#define LINE_BUFF_SIZE 4096
#define CHAR_BUFF_SIZE 5
#define ENTER 10
#define DELETE 127
#define LEFT 68
#define RIGHT 67
#define HOME 72
#define END 70

typedef struct		s_terminal
{
	struct termios	term;
	struct winsize	sz;
	char			*name;
	char			char_buff[CHAR_BUFF_SIZE];
	char			line_buff[LINE_BUFF_SIZE];
}					t_terminal;

typedef struct		s_input
{
	char			char_buff[5];
	char			line_buff[4096];
	size_t			line_size;
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
	strcat(data->line_buff, data->char_buff);
	ft_printf("%c", data->char_buff[0]);
}

int		main(void)
{
	t_terminal	config;
	t_input		data;

	if (!(constructor(&config)))
		return (0);
	while ((read(0, &data.char_buff, 5)) && data.char_buff[0] != 10)
	{
		if (data.char_buff[0] == '\E')
			break;
		if (ft_isprint(data.char_buff[0]))
			build_line(&data);
		//else if ()
	}
	return (0);
}
