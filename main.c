/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkondo <tkondo@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 13:59:22 by tkondo            #+#    #+#             */
/*   Updated: 2025/02/06 14:00:04 by tkondo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <fcntl.h>
#include <ft_global.h>
#include <ft_stdlib.h>
#include <ft_string.h>
#include <ft_unistd.h>
#include <libft.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#ifdef DEBUG
#define PRINT(...) fprintf(stderr, __VA_ARGS__)
#define EXECVP(...) execvp(__VA_ARGS__)
#else
#define PRINT(...) (void)0
#define EXECVP(...) ft_execvp(__VA_ARGS__)
#endif

void	close_fds(int *fds, size_t len)
{
	size_t	i;

	if (fds == NULL)
		return ;
	i = 0;
	while (i < len)
	{
		close(fds[i]);
		i++;
	}
}

void	eval(const char *s, int ifd, int ofd)
{
	char	**words;
	int		i;

	dup2(ifd, STDIN_FILENO);
	dup2(ofd, STDOUT_FILENO);
	close_fds((int [2]){ifd, ofd}, 2);
	PRINT("<%d %s >%d\n", ifd, s, ofd);
	words = ft_split(s, ' ');
	EXECVP(*words, words);
	perror(*words);
	exit(1);
}

int	get_infile(const char *path)
{
	return (open(path, O_RDONLY, 0777));
}

int	get_heredoc(const char *eof)
{
	return (-1);
}

int	get_outfile(const char *path)
{
	return (open(path, O_WRONLY | O_CREAT | O_TRUNC, 0777));
}

void	pipe_atexit(void)
{
	ft_delone_global("is_heredoc", NULL);
	ft_g_mmfree();
	ft_clearenv();
}

void	init_pipe(int c, char **v, char **envp)
{
	ft_set_global("is_heredoc", (void *)(long long)(ft_strcmp("heredoc",
				v[0]) == 0));
	ft_initenv(envp);
	ft_atexit(pipe_atexit);
}

void	do_pipe(char **cmds, char *cmd_last, int in, int out)
{
	int		pipe_fd[2];
	int		ofd;
	int		ifd;
	pid_t	pid;
	int		i;

	i = 0;
	ifd = in;
	PRINT("open: %d\n", ifd);
	while (*cmds != cmd_last)
	{
		pipe(pipe_fd);
		PRINT("Made pipe: %d to %d\n", pipe_fd[0], pipe_fd[1]);
		ofd = pipe_fd[1];
		pid = fork();
		if (!pid)
		{
			close(pipe_fd[0]);
			eval(*cmds, ifd, ofd);
		}
		PRINT("PID: %d\n", pid);
		close(ofd);
		close(ifd);
		ifd = pipe_fd[0];
		cmds++;
	}
	PRINT("open: %d\n", ofd);
	pid = fork();
	if (!pid)
		eval(*cmds, ifd, out);
	PRINT("PID: %d\n", pid);
	close(out);
	close(ifd);
}

int	wait_chproc(void)
{
	int	status;

	while (wait(&status) != -1)
		;
	return (status);
}

int	main(int argc, char *argv[], char *envp[])
{
	int	i;
	int	ofd;
	int	ifd;

	if (argc < 5)
		ft_exit(EXIT_FAILURE);
	init_pipe(argc + 1, argv + 1, envp);
	i = 1;
	//	if(!ft_get_global("is_heredoc"))
	ifd = get_infile(argv[i]);
	//	else
	//		ifd = get_heredoc(argv[++i]);
	ofd = get_outfile(argv[argc - 1]);
	do_pipe(argv + i + 1, argv[argc - 2], ifd, ofd);
	ft_exit(wait_chproc());
}
