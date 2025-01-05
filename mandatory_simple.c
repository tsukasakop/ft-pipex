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

int	main(int argc, char *argv[], char *envp[])
{
	pid_t	pid;
	int		fd[2];
	int	f;

	pipe(fd);
	pid = fork();
	if (pid == 0)
	{
		f = open(argv[1], O_RDONLY, 0777);
		close(fd[0]);
		dup2(f, STDIN_FILENO);
		dup2(fd[1], STDOUT_FILENO);
		eval(argv[2]);
	}
	close(fd[1]);

	pid = fork();
	if (pid == 0)
	{
		f = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0777);
		dup2(fd[0], STDIN_FILENO);
		dup2(f, STDOUT_FILENO);
		eval(argv[3]);
	}
	close(fd[0]);
	ft_initenv(envp);
	while (wait(NULL) != -1)
		;
	if (errno != ECHILD)
		perror(NULL);
	ft_exit(0);
}
