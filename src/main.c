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
	char			*line;	
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

int		main(void)
{
	t_terminal	all;

	if (!(constructor(&all)))
		return (0);
	while (1)
	{
		if ((get_next_line(0, (char**)&all.line)) > 0)
		{
			ft_printf("Your line is: %s\n", all.line);
			if (ft_strcmp(all.line, "exit") == 0)
				break ;
			free(all.line);
		}
	}
	free(all.line);
	return (0);
}
