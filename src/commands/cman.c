#include "cman.h"

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <sys/socket.h>

#define MAX_COMMANDS 10

static Command commands[MAX_COMMANDS];
static int numCommands = 0;
static int commandCount = 0;

void initializeCommandHandler() {
    numCommands = 0;
    commandCount = 0;
}

bool registerCommand(const char* name, CommandHandler handler) {
    if (numCommands < MAX_COMMANDS) {
        commands[numCommands].name = strdup(name);
        if (commands[numCommands].name == NULL) {
            fprintf(stderr, "Memory allocation error\n");
            return false;
        }

        commands[numCommands].handler = handler;
        numCommands++;
        commandCount++;

        return true;
    } else {
        fprintf(stderr, "Maximum number of commands reached\n");
        return false;
    }
}

void getCommandCount(int* countPtr) {
    *countPtr = commandCount;
}

void executeCommand(int clientSocket, const char* command) {
    char commandName[50];
    char commandArgs[50];

    int argsCount = sscanf(command, "%s %[^\n]", commandName, commandArgs);

    for (int i = 0; i < numCommands; i++) {
        if (strcmp(commands[i].name, commandName) == 0) {
            commands[i].handler(clientSocket, commandArgs);
            return;
        }
    }
    const char* errorMsg = "Invalid command. Type 'help' for a list of commands.\r\n";
    send(clientSocket, errorMsg, strlen(errorMsg), 0);
}

