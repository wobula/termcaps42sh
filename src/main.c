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
#define SAVEPOS "sc"
#define MOVERIGHT "nd"
#define MOVELEFT "le"
#define MOVEHOME "rc"

typedef struct		s_terminal
{
	struct termios	term;
	struct winsize	window_size;
	char			*name;
	char			char_buff[CHAR_BUFF_SIZE];
	char			line_buff[LINE_BUFF_SIZE];
	int				width;
	int				height;
}					t_terminal;

typedef struct		s_input
{
	char			char_buff[5];
	char			line_buff[4096];
	size_t			line_size;
	size_t			cursor_pos;
}					t_input;

void	get_win_size(t_terminal *config)
{
	ioctl(0, TIOCGWINSZ, &config->window_size);
	config->width = config->window_size.ws_col;
	config->height = config->window_size.ws_row;
}

int		raw_terminal(t_terminal *config)
{
	struct termios change;

	get_win_size(config);
	ft_dprintf(2, "width: %d, height: %d\n", config->width, config->height);
	if ((tgetent(NULL, getenv("TERM")) < 1))
		return (0);
	if ((config->name == getenv("xterm-256color")) == 0)
		ft_dprintf(2, "Opps, problem with terminal name\n");
	tcgetattr(0, &change);
	change.c_lflag &= ~(ICANON | ECHO);
	change.c_cc[VMIN] = 1;
	change.c_cc[VTIME] = 0;
	tcsetattr(0, TCSANOW, &change);
	return (1);
}

void	default_terminal(void)
{
	struct termios revert;

	tcgetattr(0, &revert);
	revert.c_lflag |= (ICANON | ECHO);	tcsetattr(0, TCSADRAIN, &revert);
	return ;
}

void	build_line(t_input *data)
{
	strcat(data->line_buff, data->char_buff);
	ft_printf("%c", data->char_buff[0]);
	data->line_size++;
	data->cursor_pos++;
}

int		ft_intputchar(int c)
{
	write(1, &c, 1);
	return (1);
}

void	my_tputs(char *cmd)
{
	int (*to_function)(int);

	to_function = ft_intputchar;
	tputs(tgetstr(cmd, NULL), 1, to_function);
}

void	move_cursor(t_input *data)
{
	if (data->char_buff[2] == RIGHT && data->cursor_pos < data->line_size)
	{
		my_tputs(MOVERIGHT);
		data->cursor_pos++;
	}
	else if (data->char_buff[2] == LEFT && data->cursor_pos > 0)
	{
		my_tputs(MOVELEFT);
		data->cursor_pos--;
	}
	else if (data->char_buff[2] == HOME)
	{
		my_tputs(MOVEHOME);
		data->cursor_pos = 0;
	}
}

int		main(void)
{
	t_terminal	config;
	t_input		data;

	if (!(raw_terminal(&config)))
		return (0);
	my_tputs(SAVEPOS);
	while ((read(0, &data.char_buff, 5)) && data.char_buff[0] != 10)
	{
		if (ft_isprint(data.char_buff[0]))
			build_line(&data);
		if (data.char_buff[0] == 27)
			move_cursor(&data);
		ft_bzero((void*)data.char_buff, 5);
	}
	ft_printf("\nline size %zu, line: %s\n", data.line_size, data.line_buff);
	default_terminal();
	return (0);
}
