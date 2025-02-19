#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <errno.h>
#include "dshlib.h"

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
    char *cmd_buff = malloc(SH_CMD_MAX * sizeof(char));
    int rc = 0;
    cmd_buff_t cmd;

    // TODO IMPLEMENT MAIN LOOP
    while (1)
    {
        printf("%s", SH_PROMPT); // Print shell prompt

        if (fgets(cmd_buff, SH_CMD_MAX, stdin) == NULL)
        {
            printf("\n");
            break; // Exit loop on EOF
        }

        // Remove trailing newline character
        cmd_buff[strcspn(cmd_buff, "\n")] = '\0';

        // TODO IMPLEMENT parsing input to cmd_buff_t *cmd_buff
        if (strlen(cmd_buff) == 0)
        {
            printf(CMD_WARN_NO_CMD);
            continue;
        }

        rc = build_cmd_buff(cmd_buff, &cmd);
        if (rc != OK)
        {
            fprintf(stderr, "Error parsing command\n");
            continue;
        }

        // TODO IMPLEMENT if built-in command, execute builtin logic for exit, cd (extra credit: dragon)
        Built_In_Cmds cmd_type = match_command(cmd.argv[0]);
        if (cmd_type == BI_CMD_EXIT)
        {
            free_cmd_buff(&cmd);
            break;
        }
        else if (cmd_type == BI_CMD_CD)
        {
            if (cmd.argc > 1)
            {
                if (chdir(cmd.argv[1]) != 0)
                {
                    perror("cd failed");
                }
            }
            free_cmd_buff(&cmd);
            continue;
        }

        // TODO IMPLEMENT if not built-in command, fork/exec as an external command
        int f_result = fork();
        if (f_result < 0)
        {
            perror("fork error");
            exit(1);
        }
        if (f_result == 0) // Child process
        {
            execvp(cmd.argv[0], cmd.argv);
            perror("fork error");
            exit(errno); // Return errno on failure
        }
        else // Parent process
        {
            wait(NULL); // Wait for child process to finish
        }

        free_cmd_buff(&cmd); // Free allocated memory
    }

    return OK;
}

/*
 * Parses the input command line into cmd_buff_t structure
 */
int build_cmd_buff(char *cmd_line, cmd_buff_t *cmd_buff)
{
    if (!cmd_line || !cmd_buff)
        return ERR_CMD_ARGS_BAD;

    cmd_buff->argc = 0;
    cmd_buff->_cmd_buffer = strdup(cmd_line); // Allocate buffer
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
            ptr++;                                  // Move past opening quote
            cmd_buff->argv[cmd_buff->argc++] = ptr; // Start of argument
            while (*ptr && (*ptr != '"' || in_quotes))
            {
                if (*ptr == '"')
                {
                    *ptr = '\0'; // Replace closing quote with null terminator
                    in_quotes = false;
                    break;
                }
                ptr++;
            }
        }
        else
        {
            cmd_buff->argv[cmd_buff->argc++] = ptr; // Start of argument
            while (*ptr && !isspace(*ptr))
                ptr++;
        }

        if (*ptr) // Null-terminate argument
        {
            *ptr = '\0';
            ptr++;
        }

        if (cmd_buff->argc >= CMD_ARGV_MAX - 1)
            break; // Prevent overflow
    }

    cmd_buff->argv[cmd_buff->argc] = NULL; // Null terminate argument list
    return OK;
}

/*
 * Matches the command with built-in commands (exit, cd)
 */
Built_In_Cmds match_command(const char *input)
{
    if (strcmp(input, EXIT_CMD) == 0)
        return BI_CMD_EXIT;
    if (strcmp(input, "cd") == 0)
        return BI_CMD_CD;

    return BI_NOT_BI;
}

/*
 * Frees dynamically allocated memory in cmd_buff_t
 */
int free_cmd_buff(cmd_buff_t *cmd_buff)
{
    if (cmd_buff->_cmd_buffer)
    {
        free(cmd_buff->_cmd_buffer);
        cmd_buff->_cmd_buffer = NULL;
    }
    return OK;
}
