1. Can you think of why we use `fork/execvp` instead of just calling `execvp` directly? What value do you think the `fork` provides?

    > **Answer**:  We use fork() before execvp() so the shell can keep running while the command runs in a separate process. If we called execvp() directly, it would replace the shell, meaning we wouldn’t be able to run more commands after that.

2. What happens if the fork() system call fails? How does your implementation handle this scenario?

    > **Answer**:  If fork() fails, it means the system couldn’t create a new process. In my implementation, if fork() returns -1, an error message ("fork error") is printed using perror(), and the shell exits with status 1 to prevent further issues.

3. How does execvp() find the command to execute? What system environment variable plays a role in this process?

    > **Answer**: execvp() searches for the command in the directories listed in the PATH environment variable. It checks each directory in PATH until it finds the executable, or returns an error if the command is not found.

4. What is the purpose of calling wait() in the parent process after forking? What would happen if we didn’t call it?

    > **Answer**:  The wait() function makes the parent process pause until the child process finishes, preventing zombie processes from accumulating. Without wait(), the child process would become a zombie, and the shell could continue running new commands without properly cleaning up finished processes.

5. In the referenced demo code we used WEXITSTATUS(). What information does this provide, and why is it important?

    > **Answer**:  WEXITSTATUS() extracts the exit status of a finished child process from the value returned by wait(). This is important because it allows the shell to check if a command executed successfully (0) or failed with an error code, which can be useful for debugging and scripting.

6. Describe how your implementation of build_cmd_buff() handles quoted arguments. Why is this necessary?

    > **Answer**:  My build_cmd_buff() function looks for quotes and keeps everything inside them as a single argument. This is important because some commands need arguments with spaces, like echo "hello world", and without handling quotes, the shell would split them into separate words.

7. What changes did you make to your parsing logic compared to the previous assignment? Were there any unexpected challenges in refactoring your old code?

    > **Answer**:  I changed my parsing logic to handle a single cmd_buff instead of a command list and made sure to properly handle spaces and quoted arguments. One challenge was making sure that spaces inside quotes were preserved while still trimming unnecessary spaces elsewhere, which took some trial and error to get right.

8. For this quesiton, you need to do some research on Linux signals. You can use [this google search](https://www.google.com/search?q=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&oq=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&gs_lcrp=EgZjaHJvbWUyBggAEEUYOdIBBzc2MGowajeoAgCwAgA&sourceid=chrome&ie=UTF-8) to get started.

- What is the purpose of signals in a Linux system, and how do they differ from other forms of interprocess communication (IPC)?

    > **Answer**: Signals are a way for the system to send messages to processes to tell them to stop, pause, or do something specific. Unlike other ways processes communicate, like pipes or shared memory, signals don’t send actual data, they just tell a process that something happened.

- Find and describe three commonly used signals (e.g., SIGKILL, SIGTERM, SIGINT). What are their typical use cases?

    > **Answer**:  
    SIGKILL (Signal 9) – This forcefully stops a process immediately and can’t be ignored or handled. It’s used when a process won’t stop normally, like with kill -9 <pid>.
    SIGTERM (Signal 15) – This is the default way to ask a process to stop nicely, so it can clean up before quitting. Unlike SIGKILL, the process can catch this and decide what to do.
    SIGINT (Signal 2) – This happens when you press Ctrl+C in the terminal, telling a program to stop. Programs can catch this signal and handle it differently instead of just quitting.

- What happens when a process receives SIGSTOP? Can it be caught or ignored like SIGINT? Why or why not?

    > **Answer**: When a process gets SIGSTOP, it completely pauses and stops running until it's resumed. Unlike SIGINT, the process can’t ignore or handle SIGSTOP, because the system forces it to pause, which is useful for debugging.
