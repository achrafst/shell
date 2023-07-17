#include "../minishell.h"

bool	is_digits_string(char *s)
{
	if (*s == '-' || *s == '+')
		s++;
	if (!*s)
		return (0);
	while (*s)
	{
		if (!ft_isdigit(*s))
			return (0);
		s++;
	}
	return (1);
}

bool	greater_than_long(char *s)
{
	int	start;
	int	len;

	start = 0;
	len = ft_strlen(s);
	if (s[start] == '-' || s[start] == '+')
		start++;
	while (s[start] == '0')
		start++;
	if (len - start < 19)
		return (0);
	if (len - start > 19)
		return (1);
	if ((*s == '+' || ft_isdigit(s[0])) &&
		ft_strcmp("9223372036854775807", s + start) < 0)
		return (1);
	if (*s == '-' && ft_strcmp("9223372036854775808", s + start) < 0)
		return (1);
	return (0);
}

void	ft_exit(t_exec *exec)
{
	t_list	*args;

	args = (*exec->cmd)->args;
	if (!args->next)
		exit(0);
	if (args && args->next && (!is_digits_string(args->next->content) || greater_than_long(args->next->content)))
	{
		ft_put_error(3, "exit", args->next->content, "numeric argument required");
		exit(255);
	}
	if (ft_lstsize(args) >= 3)
	{
		ft_put_error(2, "exit", "too many arguments");
		exit(255);
	}
	exit(ft_atoi(args->next->content));
}