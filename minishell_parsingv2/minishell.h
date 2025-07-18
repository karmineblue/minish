/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leo <leo@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 20:07:19 by ledupont          #+#    #+#             */
/*   Updated: 2025/07/01 12:08:56 by leo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <readline/readline.h>
# include "ft_printf_fd/ft_printf_fd.h"
# include "libft/libft.h"

# define UNEXPECTED "minishell: syntax error near unexpected token "
# define AMBIGUOUS_ERROR "minishell: ambiguous redirect\n"
# define AMBIGUOUS -99
# define ARG 0
# define FONCTION 1
# define PIPE 2
# define INFILE 3
# define HEREDOC 4
# define OUTFILE 5
# define APPEND 6
# define REDINFILE 31
# define REDHEREDOC 41
# define REDOUTFILE 51
# define REDAPPEND 61

typedef struct s_index
{
	int	i;
	int	w;
	int	m;
	int	h;
}	t_index;

typedef struct s_token
{
	struct s_token		*next;
	char				*str;
	int					type;
}	t_token;

typedef struct s_envlist
{
	struct s_envlist	*next;
	char				*str;
}	t_envlist;

typedef struct s_data
{
	struct s_envlist	*start;
	struct s_envlist	*end;
	struct s_token		*first;
	struct s_token		*cnext;
	char				**strs;
	char				**sptr;
	char				*line;
	char				*ex;
	char				*tmp;
	int					len;
	int					pipe;
	int					error;
}	t_data;

void		free2dstr(char **strs);
void		ft_free(void *ptr);
void		shell_check_line(t_data *data, char *s, t_index *in);
t_token		*shell_clean_line(t_data *data, t_token *cur, t_token *prev);
void		shell_clean_quote(char *str, int i);
int			shell_countwords(char *str);
int			shell_empty_expanding(t_data *data, char *s, int i, int l);
t_envlist	*shell_envlist_new(t_data *data, char **envp, int i);
void		shell_error_token(t_data *data);
int			shell_error_expanding(t_data *data, char *s, int i);
int			shell_exp_strcmp(char *envp, char *expand, int l);
int			shell_expand(t_data *data, char *s, int i);
int			shell_expand_nutcase(t_data *data, char *s, int i);
int			shell_expanding(t_data *data, char *cur, char *s, int i);
void		shell_free_envlist(t_envlist *start);
void		shell_free_token(t_token *first);
void		shell_get_envp(t_data *data, char **envp);
int			shell_get_envp_check(char **envp, int i);
void		shell_invalid_line(char *str, char *msg);
int			shell_isquote(char c);
int			shell_isredir(char *str, int i);
int			shell_space(char c);
void		shell_liberate_all(t_data *data, char *msg, int err);
void		shell_line_sep(t_data *data);
void		shell_line_set(t_data *data);
void		shell_mode_switch(char *str, t_index *in, int type);
int			shell_redircheck(char *str);
void		shell_redirection_switch(char **str, t_index *in, int i);
void		shell_sizeword(t_data *data);
void		shell_space_sep(t_data *data, char *str, t_index *in);
void		shell_spacing_sep(t_data *data, t_index *in, int type);
void		shell_split_line(t_data *data);
char		**shell_split_space(const char *s);
void		shell_to_clean(t_data *data);
int			shell_to_expand(t_data *data, char *s, int i);
int			shell_token_assign(char *str, int pt);
void		shell_token_assign_bis(t_data *data);
void		shell_token_cleaning(t_data *data);
t_token		*shell_token_inject(t_data *data, t_token *cur,\
	t_token *prev, char **strs);
t_token		*shell_token_new(t_data *data, char **strs, int i, int t);
t_token		*shell_token_sep(t_data *data, t_token *cur,\
	t_token *prev, t_index *in);
void		shell_tokenizer(t_data *data);

#endif
