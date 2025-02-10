#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "dshlib.h"

/*
 *  build_cmd_list
 *    cmd_line:     the command line from the user
 *    clist *:      pointer to clist structure to be populated
 *
 *  This function builds the command_list_t structure passed by the caller
 *  It does this by first splitting the cmd_line into commands by spltting
 *  the string based on any pipe characters '|'.  It then traverses each
 *  command.  For each command (a substring of cmd_line), it then parses
 *  that command by taking the first token as the executable name, and
 *  then the remaining tokens as the arguments.
 *
 *  NOTE your implementation should be able to handle properly removing
 *  leading and trailing spaces!
 *
 *  errors returned:
 *
 *    OK:                      No Error
 *    ERR_TOO_MANY_COMMANDS:   There is a limit of CMD_MAX (see dshlib.h)
 *                             commands.
 *    ERR_CMD_OR_ARGS_TOO_BIG: One of the commands provided by the user
 *                             was larger than allowed, either the
 *                             executable name, or the arg string.
 *
 *  Standard Library Functions You Might Want To Consider Using
 *      memset(), strcmp(), strcpy(), strtok(), strlen(), strchr()
 */
int build_cmd_list(char *cmd_line, command_list_t *clist)
{
    // check if emtpy
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
        cmds[count++] = token;
        token = strtok(NULL, PIPE_STRING);
    }

    clist->num = count;
    for (int i = 0; i < count; i++)
    {
        char *cmd = cmds[i];
        while (*cmd == SPACE_CHAR) // Trim leading spaces
            cmd++;

        char *space = strchr(cmd, SPACE_CHAR);
        if (space)
        {
            *space = '\0'; // Split command and arguments
            strncpy(clist->commands[i].exe, cmd, EXE_MAX - 1);
            strncpy(clist->commands[i].args, space + 1, ARG_MAX - 1);
        }
        else
        {
            strncpy(clist->commands[i].exe, cmd, EXE_MAX - 1);
        }
    }

    return OK;
}
