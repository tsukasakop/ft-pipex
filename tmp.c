#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void do_exec(char *cmd, int in_fd, int out_fd)
{
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(1);
    }
    if (pid == 0) {
        if (in_fd != STDIN_FILENO) {
            dup2(in_fd, STDIN_FILENO);
            close(in_fd);
        }
        if (out_fd != STDOUT_FILENO) {
            dup2(out_fd, STDOUT_FILENO);
            close(out_fd);
        }
        execlp(cmd, cmd, NULL);  // or your ft_execvp(...)
        perror(cmd);
        exit(1);
    }
    // Parent just returns; we'll wait later.
}

int main(int argc, char *argv[])
{
    // Suppose we want: argv[1] < infile | argv[2] | argv[3] | ... > outfile
    // e.g.  ./pipe infile cmd1 cmd2 cmd3 outfile
    // Then we have:   argv[1] = infile
    //                 argv[argc-1] = outfile
    //                 commands in between.

    if (argc < 4) {
        fprintf(stderr, "Usage: ./pipe infile cmd... outfile\n");
        return 1;
    }

    int in_fd = open(argv[1], O_RDONLY);
    if (in_fd < 0) {
        perror("open infile");
        return 1;
    }

    int i;
    for (i = 2; i < argc - 1; i++) {
        // If this is NOT the last command in the list:
        // create a pipe for the next command's input
        int fd[2];
        if (pipe(fd) == -1) {
            perror("pipe");
            exit(1);
        }

        // Child will read from in_fd, write to fd[1]
        do_exec(argv[i], in_fd, fd[1]);

        close(fd[1]);      // parent doesn't need write end
        close(in_fd);      // parent no longer needs old read end
        in_fd = fd[0];     // next command will read from this
    }

    // The last "command" is actually writing to outfile
    int out_fd = open(argv[argc - 1],
                      O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (out_fd < 0) {
        perror("open outfile");
        return 1;
    }

    // The final child: read from in_fd, write to out_fd
    do_exec(argv[i], in_fd, out_fd);
    close(in_fd);
    close(out_fd);

    // Wait for all children
    while (wait(NULL) > 0)
        ;
    return 0;
}
