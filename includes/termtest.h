/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   termtest.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rschramm <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/30 15:58:34 by rschramm          #+#    #+#             */
/*   Updated: 2017/11/14 13:08:03 by rschramm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TERMTEST_H
# define TERMTEST_H

# include <term.h>
# include <termios.h>
# include "../libft/includes/libft.h"

typedef struct		s_terminal
{
	struct termios	term;
	struct winsize	sz;
	char			*name;
	char			line[4096];	
}					t_terminal;

#endif
