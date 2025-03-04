1. Your shell forks multiple child processes when executing piped commands. How does your implementation ensure that all child processes complete before the shell continues accepting user input? What would happen if you forgot to call waitpid() on all child processes?

My implementation makes sure all child processes finish before the shell takes new input by calling waitpid() for each one in a loop. This prevents the shell from returning to the prompt before all commands in the pipeline have finished executing. If waitpid() were omitted, orphaned child processes (zombie processes) could accumulate, leading to resource leaks and unpredictable behavior.

2. The dup2() function is used to redirect input and output file descriptors. Explain why it is necessary to close unused pipe ends after calling dup2(). What could go wrong if you leave pipes open?

Closing unused pipe ends after calling dup2() is necessary to prevent file descriptor leaks and ensure proper process synchronization. If pipes are left open, processes might not receive an EOF signal, causing them to hang indefinitely while waiting for input. Leaving pipes open can also lead to resource exhaustion, making it impossible to create new pipes or file descriptors.

3. Your shell recognizes built-in commands (cd, exit, dragon). Unlike external commands, built-in commands do not require execvp(). Why is cd implemented as a built-in rather than an external command? What challenges would arise if cd were implemented as an external process?

The cd command is implemented as a built-in because changing the working directory affects only the process that calls it, and a child process cannot change the parent shell’s directory. If cd were an external command, it would run in a separate child process, meaning the working directory change would not persist once the process exits. This would make cd useless for navigating the shell since the parent shell would remain in the same directory.

4. Currently, your shell supports a fixed number of piped commands (CMD_MAX). How would you modify your implementation to allow an arbitrary number of piped commands while still handling memory allocation efficiently? What trade-offs would you need to consider?

To support an arbitrary number of piped commands, I would replace the fixed-size array with a dynamically allocated data structure, such as a linked list or a resizable array using realloc(). This would allow the shell to handle any number of commands while only using as much memory as needed. The trade-offs include increased complexity in memory management, potential fragmentation, and the need to carefully handle allocation failures to prevent crashes.