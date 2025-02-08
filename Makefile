pipex:
	cc main.c -o pipex -Ilibft/include -Llibft -lft -fsanitize=address -g
debug:
	cc main.c -o pipex -Ilibft/include -Llibft -lft -fsanitize=address -g -DDEBUG
