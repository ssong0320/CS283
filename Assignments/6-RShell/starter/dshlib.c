#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "dshlib.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "dshlib.h"

/****
 **** FOR REMOTE SHELL USE YOUR SOLUTION FROM SHELL PART 3 HERE
 **** THE MAIN FUNCTION CALLS THIS ONE AS ITS ENTRY POINT TO
 **** EXECUTE THE SHELL LOCALLY
 ****
 */

/*
 * Implement your exec_local_cmd_loop function by building a loop that prompts the
 * user for input.  Use the SH_PROMPT constant from dshlib.h and then
 * use fgets to accept user input.
 *
 *      while(1){
 *        printf("%s", SH_PROMPT);
 *        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL){
 *           printf("\n");
 *           break;
 *        }
 *        //remove the trailing \n from cmd_buff
 *        cmd_buff[strcspn(cmd_buff,"\n")] = '\0';
 *
 *        //IMPLEMENT THE REST OF THE REQUIREMENTS
 *      }
 *
 *   Also, use the constants in the dshlib.h in this code.
 *      SH_CMD_MAX              maximum buffer size for user input
 *      EXIT_CMD                constant that terminates the dsh program
 *      SH_PROMPT               the shell prompt
 *      OK                      the command was parsed properly
 *      WARN_NO_CMDS            the user command was empty
 *      ERR_TOO_MANY_COMMANDS   too many pipes used
 *      ERR_MEMORY              dynamic memory management failure
 *
 *   errors returned
 *      OK                     No error
 *      ERR_MEMORY             Dynamic memory management failure
 *      WARN_NO_CMDS           No commands parsed
 *      ERR_TOO_MANY_COMMANDS  too many pipes used
 *
 *   console messages
 *      CMD_WARN_NO_CMD        print on WARN_NO_CMDS
 *      CMD_ERR_PIPE_LIMIT     print on ERR_TOO_MANY_COMMANDS
 *      CMD_ERR_EXECUTE        print on execution failure of external command
 *
 *  Standard Library Functions You Might Want To Consider Using (assignment 1+)
 *      malloc(), free(), strlen(), fgets(), strcspn(), printf()
 *
 *  Standard Library Functions You Might Want To Consider Using (assignment 2+)
 *      fork(), execvp(), exit(), chdir()
 */
int exec_local_cmd_loop()
{
    char *cmd_buff = malloc(SH_CMD_MAX * sizeof(char)); // Allocate buffer
    if (!cmd_buff)
    {
        perror("malloc failed");
        return ERR_MEMORY;
    }

    command_list_t clist;

    while (1)
    {
        printf("%s", SH_PROMPT);

        if (fgets(cmd_buff, SH_CMD_MAX, stdin) == NULL)
        {
            printf("\n");
            break; // Exit loop on EOF
        }

        // Remove trailing newline character
        cmd_buff[strcspn(cmd_buff, "\n")] = '\0';

        if (strlen(cmd_buff) == 0)
        {
            fprintf(stderr, CMD_WARN_NO_CMD);
            continue;
        }

        // Parse command into command list
        int rc = build_cmd_list(cmd_buff, &clist);
        if (rc == WARN_NO_CMDS)
        {
            fprintf(stderr, CMD_WARN_NO_CMD);
            continue;
        }
        else if (rc == ERR_TOO_MANY_COMMANDS)
        {
            fprintf(stderr, CMD_ERR_PIPE_LIMIT, CMD_MAX);
            continue;
        }

        // Built-in command handling (cd, exit)
        if (clist.num == 1)
        {
            char *cmd = clist.commands[0].argv[0];

            if (cmd == NULL)
                continue; // Ignore empty commands

            if (strcmp(cmd, EXIT_CMD) == 0)
            {
                printf("exiting...\n");
                break;
            }
            else if (strcmp(cmd, "cd") == 0)
            {
                if (clist.commands[0].argc < 2)
                {
                    fprintf(stderr, "cd: missing argument\n");
                }
                else if (chdir(clist.commands[0].argv[1]) != 0)
                {
                    perror("cd failed");
                }
                continue;
            }
        }

        // Execute pipeline (handles single and multiple commands)
        execute_pipeline(&clist);
    }

    free(cmd_buff);
    return OK;
}

int execute_pipeline(command_list_t *clist)
{
    int num_commands = clist->num;
    int pipes[num_commands - 1][2];
    pid_t pids[num_commands];
    int exit_status = 0; // Track if any command fails

    // Create pipes
    for (int i = 0; i < num_commands - 1; i++)
    {
        if (pipe(pipes[i]) == -1)
        {
            perror("pipe");
            return ERR_MEMORY;
        }
    }

    // Fork and execute each command in the pipeline
    for (int i = 0; i < num_commands; i++)
    {
        if (clist->commands[i].argv[0] == NULL)
        {
            fprintf(stderr, "Error: Empty command detected\n");
            return WARN_NO_CMDS;
        }

        pids[i] = fork();
        if (pids[i] < 0)
        {
            perror("fork");
            return ERR_MEMORY;
        }

        if (pids[i] == 0) // Child process
        {
            // Set up input pipe (if not the first command)
            if (i > 0)
                dup2(pipes[i - 1][0], STDIN_FILENO);

            // Set up output pipe (if not the last command)
            if (i < num_commands - 1)
                dup2(pipes[i][1], STDOUT_FILENO);

            // Close all pipes in child
            for (int j = 0; j < num_commands - 1; j++)
            {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }

            // Execute command
            execvp(clist->commands[i].argv[0], clist->commands[i].argv);
            perror("execvp failed");
            exit(127); // Standard exit code for command not found
        }
    }

    // Close pipes in parent process
    for (int i = 0; i < num_commands - 1; i++)
    {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }

    // Wait for all child processes & track failures
    for (int i = 0; i < num_commands; i++)
    {
        int status;
        waitpid(pids[i], &status, 0);
        if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
        {
            exit_status = WEXITSTATUS(status); // Track first failure
        }
    }

    return exit_status; // Return non-zero if any command failed
}

int build_cmd_list(char *cmd_line, command_list_t *clist)
{
    if (strlen(cmd_line) == 0)
    {
        return WARN_NO_CMDS;
    }

    memset(clist, 0, sizeof(command_list_t));

    char *token;
    char *cmds[CMD_MAX];
    int count = 0;

    // Tokenize input based on '|'
    token = strtok(cmd_line, PIPE_STRING);
    while (token != NULL)
    {
        if (count >= CMD_MAX)
        {
            return ERR_TOO_MANY_COMMANDS;
        }

        // Trim leading spaces
        while (*token == ' ')
            token++;

        // Ensure command isn't empty
        if (*token == '\0')
        {
            return WARN_NO_CMDS;
        }

        cmds[count++] = token;
        token = strtok(NULL, PIPE_STRING);
    }

    clist->num = count;
    for (int i = 0; i < count; i++)
    {
        if (build_cmd_buff(cmds[i], &clist->commands[i]) != OK)
        {
            return ERR_CMD_OR_ARGS_TOO_BIG;
        }
    }

    return OK;
}

int build_cmd_buff(char *cmd_line, cmd_buff_t *cmd_buff)
{
    if (!cmd_line || !cmd_buff)
        return ERR_CMD_ARGS_BAD;

    cmd_buff->argc = 0;
    cmd_buff->_cmd_buffer = strdup(cmd_line);
    if (!cmd_buff->_cmd_buffer)
        return ERR_MEMORY;

    char *ptr = cmd_buff->_cmd_buffer;
    bool in_quotes = false;

    while (*ptr)
    {
        while (isspace(*ptr))
            ptr++; // Skip leading spaces

        if (*ptr == '"') // Detect quoted string
        {
            in_quotes = true;
            ptr++;
            cmd_buff->argv[cmd_buff->argc++] = ptr;
            while (*ptr && (*ptr != '"' || in_quotes))
            {
                if (*ptr == '"')
                {
                    *ptr = '\0';
                    in_quotes = false;
                    break;
                }
                ptr++;
            }
        }
        else
        {
            cmd_buff->argv[cmd_buff->argc++] = ptr;
            while (*ptr && !isspace(*ptr))
                ptr++;
        }

        if (*ptr)
        {
            *ptr = '\0';
            ptr++;
        }

        if (cmd_buff->argc >= CMD_ARGV_MAX - 1)
            break;
    }

    cmd_buff->argv[cmd_buff->argc] = NULL;

    return OK;
}
