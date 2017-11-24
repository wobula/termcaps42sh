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
#define UP 65
#define DOWN 66
#define LEFT 68
#define RIGHT 67
#define HOME 72
#define END 70

#define SAVEPOS "sc"
#define MOVERIGHT "nd"
#define MOVELEFT "le"
#define MOVEUP "up"
#define MOVEDN "do"
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
	t_terminal		*config;
}					t_input;

typedef struct		s_cmds
{
	struct s_cmds	*next;
	struct s_cmds	*prev;
	struct s_cmds	*end;
	char			*cmd;
}					t_cmds;

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

void	move_home(t_input *data)
{
	while (data->cursor_row > 0)
	{
		my_tputs(MOVEUP);
		data->cursor_row--;
	}
	while (data->cursor_col > 0)
	{
		my_tputs(MOVELEFT);
		data->cursor_col--;
	}
}

void	move_end(t_input *data)
{
	while (data->cursor_row < data->end_row)
	{
		my_tputs(MOVEDN);
		data->cursor_row++;
	}
	if (data->cursor_col > data->end_col)
	{
		while (data->cursor_col > data->end_col)
		{
			my_tputs(MOVELEFT);
			data->cursor_col--;
		}
	}
	else if (data->cursor_col < data->end_col)
	{
		while (data->cursor_col < data->end_col)
		{
			my_tputs(MOVERIGHT);
			data->cursor_col++;
		}
	}
}

void	move_right(t_input *data)
{
	size_t size;

	size = (data->cursor_col * data->config->width) + data->cursor_col;
	if (data->cursor_col + 1 == data->config->width)
	{
		my_tputs(MOVEDN);
	}
	else if (data->cursor_col < size)
		my_tputs(MOVERIGHT);
}

void	move_cursor(t_input *data)
{
	if (data->char_buff[2] == RIGHT && data->cursor_col < data->line_size)
	{
		move_right(data);
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
		move_end(data);
		data->cursor_pos = data->line_size;
	}
	else if (data->char_buff[2] == UP)
	{
		ft_printf("Pressed up\n");
	}
	else if (data->char_buff[2] == DOWN)
	{
		ft_printf("Pressed down\n");
	}
}

void	get_window_size(t_terminal *config)
{
	ioctl(0, TIOCGWINSZ, &config->window_size);
	config->width = config->window_size.ws_col;
	config->height = config->window_size.ws_row;
}

int		raw_terminal(t_terminal *config, t_input *data)
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
	data->config = config;
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

void	delete(t_input *data)
{
	my_tputs(MOVELEFT);
	my_tputs("dc");
	data->line_buff[data->cursor_pos] = '\0';
	strcat(data->line_buff, data->line_buff + data->cursor_pos + 1);
	data->line_size--;
	data->cursor_pos--;
}

void	insert(t_input *data)
{
	char	buff[LINE_BUFF_SIZE];

	ft_bzero((void*)buff, LINE_BUFF_SIZE);
	strcpy(buff, data->char_buff);
	strcat(buff, data->line_buff + data->cursor_pos);
	data->line_buff[data->cursor_pos] = '\0';
	strcat(data->line_buff, buff);
	my_tputs("im");
	ft_putstr(data->char_buff);
	my_tputs("ei");
	data->cursor_pos++;
	data->line_size++;
}

void	history_constructor(t_cmds *head)
{
	head->prev = NULL;
	head->next = NULL;
	head->end = NULL;
	head->cmd = NULL;
}

void	add_cmd(t_cmds *head, char *cmd)
{
	if (!head->cmd)
	{
		head->cmd = ft_strdup(cmd);
		return ;
	}
	else if (!head->next)
	{
		head->next = (t_cmds*)ft_memalloc(sizeof(t_cmds));
		head->next->next = NULL;
		head->next->prev = head;
		head->end = head->next;
		head->next->cmd = ft_strdup(cmd);
		return ;
	}
	head->end->next = (t_cmds*)ft_memalloc(sizeof(t_cmds));
	head->end->next->next = NULL;
	head->end->next->prev = head->end;
	head->end->next->cmd = ft_strdup(cmd);
	head->end = head->end->next;
}

void	vizualize_history(t_cmds head)
{
	t_cmds *tmp;

	tmp = &head;
	ft_printf("\n\nHistory:\n");
	while (tmp)
	{
		ft_printf("%s\n", tmp->cmd);
		tmp = tmp->next;
	}
	ft_putchar('\n');
}

void	cleanup_history(t_cmds *head)
{
	t_cmds *tmp;
	t_cmds *prev;
	if (head->cmd)
		free(head->cmd);
	tmp = head->next;
	{
		while (tmp)
		{
			prev = tmp;
			tmp = tmp->next;
			free(prev->cmd);
			free(prev);
		}
	}
}

void	input_constructor(t_input *data)
{
	ft_bzero(data->char_buff, 5);
	ft_bzero(data->line_buff, 4096);
	data->line_size = 0;
	data->cursor_pos = 0;
}

int		main(void)
{
	t_terminal	config;
	t_input		data;
	t_cmds		history;

	if (!(raw_terminal(&config, &data)))
		return (0);
	history_constructor(&history);
	while (1)
	{
		input_constructor(&data);
		while ((read(0, &data.char_buff, 5)) && data.char_buff[0] != ENTER)
		{
			get_terminal_meta(&config, &data);
			if (ft_isprint(data.char_buff[0]))
				insert(&data);
			else if (data.char_buff[0] == DELETE && data.cursor_pos != 0)
				delete(&data);
			else if (data.char_buff[0] == 27)
				move_cursor(&data);
			ft_bzero((void*)data.char_buff, 5);
		}
		if (data.line_buff[0] != '\0')
			add_cmd(&history, data.line_buff);
		vizualize_history(history);
		if (ft_strcmp(data.line_buff, "exit") == 0)
			break;
	}
	cleanup_history(&history);
	get_terminal_meta(&config, &data);
	ft_printf("\nline size %zu, line: %s\n", data.line_size, data.line_buff);
	ft_printf("Cursor position - row %d, col %d\n", data.cursor_row, data.cursor_col);
	ft_printf("End position - row %d, col %d\n", data.end_row, data.end_col);
	default_terminal();
	return (0);
}
