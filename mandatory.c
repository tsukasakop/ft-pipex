#include <errno.h>
#include <fcntl.h>
#include <ft_stdlib.h>
#include <ft_unistd.h>
#include <libft.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

void	eval(const char *s)
{
	char	**words;
	int		i;

	words = ft_split(s, ' ');
	execvp(*words, words);
	i = 0;
	while (words[i])
	{
		free(words[i]);
		i++;
	}
	free(words);
	exit(1);
}

int	pp2infile(char *argv, char *path)
{
	int		fd[2];
	pid_t	pid;

	pipe(fd);
	pid = fork();
	if (pid == 0)
	{
		int f = open(path, O_RDONLY, 0777);
		if (f == -1)
			ft_exit(1);
		close(fd[0]);
		dup2(f, STDIN_FILENO);
		dup2(fd[1], STDOUT_FILENO);
		eval(argv);
	}
	close(fd[1]);
	return fd[0];
}

void	pp2outfile(char *argv, char *path, int ofd)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		int f = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0777);
		if (f == -1)
			ft_exit(1);
		dup2(ofd, STDIN_FILENO);
		dup2(f, STDOUT_FILENO);
		eval(argv);
	}
	close(ofd);;
}

int	main(int argc, char *argv[], char *envp[])
{
	int	i;
	int	fd;

	fd = pp2infile(argv[2], argv[1]);
	pp2outfile(argv[3],argv[4], fd);
	while (wait(NULL) != -1)
		;
	if (errno != ECHILD)
		perror(NULL);
}
