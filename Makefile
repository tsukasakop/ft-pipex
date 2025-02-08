pipex:
	make -C libft
	cc main.c -o pipex -Ilibft/include -Llibft -lft -fsanitize=address -g
debug:
	make -C libft
	cc main.c -o pipex -Ilibft/include -Llibft -lft -fsanitize=address -g -DDEBUG
