#ifndef CMAN_H
#define CMAN_H

#include <stdbool.h>

typedef void (*CommandHandler)(int clientSocket, const char* args);

typedef struct {
    const char* name;
    CommandHandler handler;
} Command;

void initializeCommandHandler();
bool registerCommand(const char* name, CommandHandler handler);
void executeCommand(int clientSocket, const char* command);
void getCommandCount(int* countPtr);

#endif
