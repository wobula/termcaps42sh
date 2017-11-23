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
#define LINEUP "up"
#define LINESTART "cr"

typedef struct		s_terminal
{
	struct termios	term;
	struct winsize	window_size;
	char			*name;
	char			char_buff[CHAR_BUFF_SIZE];
	char			line_buff[LINE_BUFF_SIZE];
	size_t			width;
	size_t			height;
}					t_terminal;

typedef struct		s_input
{
	char			char_buff[5];
	char			line_buff[4096];
	size_t			line_size;
	size_t			cursor_pos;
	size_t			cursor_col;
	size_t			cursor_row;
	size_t			end_col;
	size_t			end_row;
}					t_input;

int		ft_intputchar(int c)
{
	write(1, &c, 1);
	return (1);
}

void	second_tputs(char *cmd)
{
	int (*to_function)(int);

	to_function = ft_intputchar;
	tputs(cmd, 1, to_function);
}

void	my_tputs(char *cmd)
{
	int (*to_function)(int);

	to_function = ft_intputchar;
	tputs(tgetstr(cmd, NULL), 1, to_function);
}

void	build_line(t_input *data)
{
	strcat(data->line_buff, data->char_buff);
	ft_printf("%c", data->char_buff[0]);
	data->line_size++;
	data->cursor_pos++;
}

void	move_home(t_input *data)
{
	while (data->cursor_row > 0)
	{
		my_tputs(LINEUP);
		data->cursor_row--;
	}
	while (data->cursor_col > 0)
	{
		my_tputs(MOVELEFT);
		data->cursor_col--;
	}
}

void	move_cursor(t_input *data)
{
	if (data->char_buff[2] == RIGHT && data->cursor_col < data->line_size)
	{
		my_tputs(MOVERIGHT);
		data->cursor_pos++;
	}
	else if (data->char_buff[2] == LEFT && data->cursor_col > 0)
	{
		my_tputs(MOVELEFT);
		data->cursor_pos--;
	}
	else if (data->char_buff[2] == HOME)
	{
		move_home(data);
		data->cursor_pos = 0;
	}
	else if (data->char_buff[2] == END)
	{
		ft_printf("-->%s\n", tgetstr("cm", NULL));
		//tgoto(tgetstr("cm", NULL), data->line_size, 20);
		second_tputs(tgoto(tgetstr("cm", NULL), data->line_size, 20));
		//my_tputs(tgoto(tgetstr("cm", NULL), 20, 50));
		data->cursor_pos = data->line_size;
	}
}

void	get_window_size(t_terminal *config)
{
	ioctl(0, TIOCGWINSZ, &config->window_size);
	config->width = config->window_size.ws_col;
	config->height = config->window_size.ws_row;
}

int		raw_terminal(t_terminal *config)
{
	struct termios change;

	if ((tgetent(NULL, getenv("TERM")) < 1))
		return (0);
	if ((config->name == getenv("xterm-256color")) == 0)
		ft_dprintf(2, "Opps, problem with terminal name\n");
	tcgetattr(0, &change);
	change.c_lflag &= ~(ICANON | ECHO);
	change.c_cc[VMIN] = 1;
	change.c_cc[VTIME] = 0;
	tcsetattr(0, TCSANOW, &change);
	get_window_size(config);
	my_tputs(SAVEPOS);
	return (1);
}

void	default_terminal(void)
{
	struct termios revert;

	tcgetattr(0, &revert);
	revert.c_lflag |= (ICANON | ECHO);
	tcsetattr(0, TCSADRAIN, &revert);
	return ;
}

void	get_cursor_pos(t_terminal *config, t_input *data)
{
	size_t blah;
	size_t row;

	blah = data->cursor_pos;
	row = 0;
	while (blah > config->width)
	{
		row++;
		blah -= config->width;
	}
	data->cursor_col = blah;
	data->cursor_row = row;
	blah = data->line_size;
	row = 0;
	while (blah > config->width)
	{
		row++;
		blah -= config->width;
	}
	data->end_col = blah;
	data->end_row = row;
}

void	get_terminal_meta(t_terminal *config, t_input *data)
{
	get_window_size(config);
	get_cursor_pos(config, data);
}

int		main(void)
{
	t_terminal	config;
	t_input		data;

	if (!(raw_terminal(&config)))
		return (0);
	ft_dprintf(2, "window dimensions - h: %zu, w: %zu\n", config.height, config.width);
	while ((read(0, &data.char_buff, 5)) && data.char_buff[0] != 10)
	{
		get_terminal_meta(&config, &data);
		if (ft_isprint(data.char_buff[0]))
			build_line(&data);
		if (data.char_buff[0] == 27)
			move_cursor(&data);
		ft_bzero((void*)data.char_buff, 5);
	}
	get_terminal_meta(&config, &data);
	ft_printf("\nline size %zu, line: %s\n", data.line_size, data.line_buff);
	ft_printf("Cursor position - row %d, col %d\n", data.cursor_row, data.cursor_col);
	ft_printf("End position - row %d, col %d\n", data.end_row, data.end_col);
	default_terminal();
	return (0);
}
