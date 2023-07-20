/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_herdocs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asettar <asettar@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 04:16:48 by zel-bouz          #+#    #+#             */
/*   Updated: 2023/07/19 23:41:31 by asettar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_perror(char *err)
{
	perror(err);
	exit(EXIT_FAILURE);
}

char	*ft_readline(char *prompt)
{
	char	*line;

	ft_printf("%s", prompt);
	line = get_next_line(0);
	if (line)
		line[ft_strlen(line) - 1] = '\0';
	return (line);
}

void	her_handler(int s)
{
	(void)s;
	g_glob.her = 1;
	g_glob.status = 1;
	rl_on_new_line();
	rl_replace_line("", 0);
	ioctl(0, TIOCSTI, "\4");
}

int	create_herdoc(t_redir *redir, t_env *env)
{
	int		fd[2];
	char	*line;

	if (pipe(fd) == -1)
		return (ft_put_error(2, "pipe", strerror(errno)),
			g_glob.status = 1, -1);
	while (1)
	{
		signal(SIGINT, her_handler);
		line = readline("herdoc $>");
		if (!line || !ft_strcmp(line, redir->file))
			return (free(line), close(fd[1]), fd[0]);
		if (redir->flag && ft_strchr(line, '$'))
			line = replace_dolar(line, env);
		write(fd[1], line, ft_strlen(line));
		write(fd[1], "\n", 1);
		free(line);
	}
	return (free(line), close(fd[1]), fd[0]);
}

void	exec_herdocs(t_cmd	*cmd, t_env *env)
{
	t_redir	*itr;

	while (cmd)
	{
		itr = cmd->redir;
		while (itr)
		{
			if (itr->type == HER)
			{
				itr->fd = create_herdoc(itr, env);
				if (g_glob.her == 1)
					return ;
				if (itr->fd == -1)
				{
					cmd->triger = -1;
					break ;
				}
			}
			itr = itr->next;
		}
		cmd = cmd->next;
	}
}
