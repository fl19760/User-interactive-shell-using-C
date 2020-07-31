My shell supports the following commands:
--> Shell builtins like ls, cd, echo, pwd.
--> All reamining commands supported by shell.
--> Background and foreground processes.
--> Input and output redirections.
--> Piping.
--> Other user defined commands such as pinfo, setenv, unsetenv, jobs, kjob, fg, bg, overkill, quit, CTRL-Z, CTRL-C and ​ Command Recall using ‘UP’ arrow key.


The files used are:
1) shell.c (main program and takes care of redirections)
2) cd.h (handles cd)
3) ls.h (handles ls)
4) pid.h (handles pinfo related functions)
5) execute.h (calls the functions executing all the commands and takes care of forking)
6) interrupts.h (handles the interrupt handler functions)
7) parsing.h (handles string parsing)

Running the shell:
-->make
-->./shell