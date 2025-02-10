1. In this assignment I suggested you use `fgets()` to get user input in the main while loop. Why is `fgets()` a good choice for this application?

    > **Answer**: fgets() ensures that the input does not exceed the allocated buffer size. Additionally, fgets() allows us to detect EOF, making it easier to support headless execution

2. You needed to use `malloc()` to allocte memory for `cmd_buff` in `dsh_cli.c`. Can you explain why you needed to do that, instead of allocating a fixed-size array?

    > **Answer**:  Using malloc() allows us to dynamically allocate memory, which is more flexible than a fixed-size array. This ensures that we can manage memory based on the system’s needs and avoid stack overflows that may occur with large fixed-size arrays. Additionally, dynamic allocation enables better memory management for future modifications, such as resizing the buffer or supporting different input sizes dynamically.


3. In `dshlib.c`, the function `build_cmd_list(`)` must trim leading and trailing spaces from each command before storing it. Why is this necessary? If we didn't trim spaces, what kind of issues might arise when executing commands in our shell?

    > **Answer**:  Trimming spaces ensures commands are parsed and executed correctly. Without trimming, unnecessary spaces could cause incorrect command recognition or empty arguments, leading to unexpected behavior. For example, " ls " might be misinterpreted as " ls" instead of "ls", potentially causing execution failures.

4. For this question you need to do some research on STDIN, STDOUT, and STDERR in Linux. We've learned this week that shells are "robust brokers of input and output". Google _"linux shell stdin stdout stderr explained"_ to get started.

- One topic you should have found information on is "redirection". Please provide at least 3 redirection examples that we should implement in our custom shell, and explain what challenges we might have implementing them.

    > **Answer**: 
    1. Output Redirection (>, >>): Saves command output to a file instead of displaying it in the terminal (ls > output.txt). The challenge is handling file descriptors and ensuring data integrity.

    2. Input Redirection (<): Reads input from a file instead of the keyboard (sort < input.txt). The challenge is checking file existence and handling permission errors.
    
    3. Error Redirection (2>): Separates error messages from normal output (ls non_existing_dir 2> errors.log). The challenge is correctly managing STDERR without affecting STDOUT.

- You should have also learned about "pipes". Redirection and piping both involve controlling input and output in the shell, but they serve different purposes. Explain the key differences between redirection and piping.

    > **Answer**:  Redirection works with files, sending output to or reading input from them. Piping connects two commands, passing the output of one as input to another. While redirection stores data, piping is used for real-time command chaining.

- STDERR is often used for error messages, while STDOUT is for regular output. Why is it important to keep these separate in a shell?

    > **Answer**:  Keeping STDOUT and STDERR separate improves debugging and logging. This prevents error messages from mixing with valid command output, allowing users to handle errors independently 

- How should our custom shell handle errors from commands that fail? Consider cases where a command outputs both STDOUT and STDERR. Should we provide a way to merge them, and if so, how?

    > **Answer**: Our shell should detect errors using return codes from exec() and inform the user appropriately. If needed, we can merge STDOUT and STDERR using 2>&1. This ensures both normal output and error messages are captured together in the same file or stream.