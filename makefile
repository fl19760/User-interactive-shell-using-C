output: shell.c
	gcc -g shell.c -o shell -L/usr/include -lreadline

# shell.o: shell.c
# 	gcc -c shell.c -L/usr/include -lreadline
clean:
	rm shell	
