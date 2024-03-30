#include "help_command.h"
#include "cman.h"

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

void executeHelpCommand(int clientSocket, const char* args) {

    int commandCount;
    getCommandCount(&commandCount);
    char helpMsg[200];

    snprintf(helpMsg, sizeof(helpMsg),"Help Message\r\n");

    send(clientSocket, helpMsg, strlen(helpMsg), 0);
}

__attribute__((constructor))
void registerHelpCommand() {
    registerCommand("help", executeHelpCommand);
}
