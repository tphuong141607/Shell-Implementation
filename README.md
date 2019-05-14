# yesCODE--sushi

This implemented SHELL is created in March, 2019.

### The sushi SHELL provides some real SHELL's basic functionalities such as:

#### 1. Validate, store and save commands from the command line/terminal to history. Check the history my typing `!`
#### 2. The ability to start other programs
#### 3. Background and foreground execution, memory cleanup, and environment management
#### 4. Software pipes: 
Shell expression A | B means that the standard output of process A is redirected through a pipe to the standard input of process B. Neither process “knows” that the I/O’s have been reconnected. 

The code of A or B has not been modified. Pipes can be chained
potentially indefinitely: A | B | C | … | Z. This core Unix/Linux
construct is called “pipes and filters” architecture, where data is
treated as fluid and processes filter and modify it as it flows
through them. 

#### 5. Shell script support
A shell can be used as a script interpreter. If any parameters are
passed to the shell via argv, they shall be treated as file names.

#### 6. Shell prompt
The shell prompt is controlled by the environmental variable $PS1, you can change the environmental variable by setting PS1 = `Your desire name`.

#### 7. Input/output redirection
The following command line reads file sushi.c with cat,
counts the number of lines in the file with wc -l, and appends the
results to the end of lines.txt. If the latter file does not exist,
it is first created: `cat < sushi.c | wc -l >> lines.txt`

#### 8. Change the current working directory using `cd`
#### 9. Show the current working directory using `pwd`
