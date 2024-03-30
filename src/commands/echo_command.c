#include "echo_command.h"
#include <string.h>
#include <sys/socket.h>

const char* echoMsg = "Echo\r\n";

void executeEchoCommand(int clientSocket, const char* args) {
    send(clientSocket, echoMsg, strlen(echoMsg), 0);
}
__attribute__((constructor))
void registerEchoCommand() {
    registerCommand("help", executeEchoCommand);
}
