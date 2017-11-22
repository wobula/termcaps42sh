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

int		constructor(t_terminal *all)
{
	if ((tgetent(NULL, getenv("TERM")) < 1))
		return (0);
	if ((all->name == getenv("xterm-256color")) == 0)
		ft_dprintf(2, "Opps, problem with terminal name\n");
	tcgetattr(0, &all->term);
	all->term.c_lflag &= ~(ICANON | ECHO);
	all->term.c_cc[VMIN] = 1;
	all->term.c_cc[VTIME] = 0;
	tcsetattr(0, TCSANOW, &all->term);
	return (1);
}

void	build_line(t_terminal *all)
{
	ft_printf("inside build_line: %c\n", all->char_buff[0]);
	strcat(all->line_buff, all->char_buff);
	ft_printf("Voila, your line: %s\n", all->line_buff);
}

int		main(void)
{
	t_terminal	all;

	if (!(constructor(&all)))
		return (0);
	while ((read(0, &all.char_buff, 5)) && all.char_buff[0] != 10)
	{
		ft_printf("You just entered: %c\n", all.char_buff[0]);
		if (all.char_buff[0] == '\E')
			break;
		if (ft_isprint(all.char_buff[0]))
			build_line(&all);
	}
	return (0);
}
