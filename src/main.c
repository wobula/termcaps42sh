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

#include "../includes/term.h"

int		main(void)
{
	char	*line;

	while (1)
	{
		if ((get_next_line(0, &line)) > 0)
		{
			ft_printf("Your line is: %s\n", line);
			free(line);
		}
	}
	return (0);
}
