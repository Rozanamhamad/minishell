# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

This is the 42 School **minishell** project - a recreation of a basic shell (like bash). The goal is to create a simple shell that handles commands, pipes, redirections, built-ins, and environment variables.

## Build System

Mandatory Makefile rules that must be present:
- `make` or `make all` - Builds the minishell executable
- `make clean` - Removes object files
- `make fclean` - Removes object files and executable  
- `make re` - Rebuilds everything from scratch
- `make bonus` - Compiles bonus features (if implemented)

The project compiles with `-Wall -Wextra -Werror` flags using `cc` and links against libft and readline.

## Testing

The project includes a comprehensive test suite in `minishell_tester/`:

- `./minishell_tester/tester` - Runs all mandatory tests
- `./minishell_tester/tester builtins` - Tests built-in commands
- `./minishell_tester/tester pipes` - Tests pipe functionality
- `./minishell_tester/tester redirects` - Tests I/O redirection
- `./minishell_tester/tester wildcards` - Tests bonus wildcard features
- `./minishell_tester/tester syntax` - Tests syntax error handling
- `./minishell_tester/tester os_specific` - Tests OS-specific behavior

Manual test suggestions are available in `minishell_tester/manual_tests/`.

## Mandatory Requirements

### Core Shell Features
- Display prompt when waiting for commands
- Working command history
- Search and launch executables (PATH, relative, absolute paths)
- Handle pipes (`|`) - connect command output to next command input
- Environment variable expansion (`$VAR`, `$?` for exit status)

### Quote Handling
- **Single quotes (`'`)**: Prevent interpretation of ALL metacharacters
- **Double quotes (`"`)**: Prevent interpretation except `$` (dollar sign)
- Do NOT handle unclosed quotes or unsupported special characters (`\`, `;`)

### Redirections
- `<` - Input redirection
- `>` - Output redirection  
- `<<` - Heredoc (read until delimiter, don't update history)
- `>>` - Append output redirection

### Signal Handling
- **ctrl-C**: Display new prompt on new line (interactive mode)
- **ctrl-D**: Exit the shell (interactive mode)
- **ctrl-\\**: Do nothing (interactive mode)
- Use AT MOST one global variable for signal number only

### Built-in Commands (Must Implement)
- `echo` with `-n` option
- `cd` with relative or absolute path only
- `pwd` with no options
- `export` with no options
- `unset` with no options
- `env` with no options or arguments
- `exit` with no options

## Architecture

This implementation follows a three-phase architecture:

### 1. Tokenization (`tokenization/`)
- **Input Processing**: Converts raw input string into tokens
- **Quote Handling**: Manages single and double quotes (`handle_quotes.c`)
- **Token Creation**: Creates linked list of tokens with types (`node_creation.c`)
- **Core Logic**: Main tokenization handling (`handling.c`)

### 2. Parsing (`parsing/`)
- **AST Construction**: Builds Abstract Syntax Tree from tokens (`creation.c`)
- **Environment Handling**: Manages environment variables (`env.c`)
- **Variable Expansion**: Expands $VAR and other variables (`expansion.c`)
- **Quote Processing**: Removes quotes and processes quoted strings (`quotation.c`)
- **Core Parsing**: Main parsing logic (`handling.c`)

### 3. Execution (`execution/`)
- **Command Execution**: Handles simple command execution (`exec_cmd.c`)
- **Pipe Implementation**: Manages pipeline execution (`exec_pipe.c`)
- **Heredoc Processing**: Handles << redirection (`handel_heredoc.c`)
- **I/O Redirection**: Manages file redirections (`rediretion_exec.c`)

### Built-in Commands (`builtsin/`)
All mandatory built-ins implemented as separate modules:
- `echo.c`, `cd.c`, `pwd.c`, `export.c`, `unset.c`, `env.c`, `exit.c`
- Environment utilities for variable management (`env_utils.c`)

## Key Data Structures

- **t_tokenlist**: Doubly-linked list of tokens with head/tail pointers
- **t_ast_node**: AST nodes supporting commands, pipes, and redirections
- **t_myenv**: Environment wrapper containing env array and exit code
- **t_type**: Token/node types (T_PIPE, T_SCMD, T_REDIR_*, T_HEREDOC)

## Allowed External Functions

```c
readline, rl_clear_history, rl_on_new_line, rl_replace_line, rl_redisplay, add_history,
printf, malloc, free, write, access, open, read, close, fork, wait, waitpid, wait3, wait4,
signal, sigaction, sigemptyset, sigaddset, kill, exit, getcwd, chdir, stat, lstat, fstat,
unlink, execve, dup, dup2, pipe, opendir, readdir, closedir, strerror, perror, isatty,
ttyname, ttyslot, ioctl, getenv, tcsetattr, tcgetattr, tgetent, tgetflag, tgetnum, tgetstr,
tgoto, tputs
```

## Dependencies

- **libft**: Custom C library in `libft/` directory (must be compiled by main Makefile)
- **readline**: For command line input and history
- **Standard C libraries**: System calls for process/file management

## Important Constraints

- **Memory Management**: All heap-allocated memory must be properly freed
- **Error Handling**: No segfaults, bus errors, or double frees allowed
- **Global Variables**: At most ONE global variable (for signal number only)
- **Reference Behavior**: When in doubt, bash is the reference implementation
- **Norm Compliance**: Must follow 42 coding standards
- **Readline Leaks**: Readline may leak memory (acceptable), but your code must not

## Development Notes

- Use bash as reference for expected behavior
- Limit implementation to subject requirements only
- Focus on mandatory part before attempting bonus
- Test thoroughly with the provided test suite
- The main loop follows: input → tokenize → parse → execute