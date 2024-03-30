#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "commands/cman.h"
#include "commands/echo_command.h"
#include "commands/help_command.h"

#define PORT 24
#define TCP 8080
#define MAX_CLIENTS 5
#define BUFFER_SIZE 1024

void handleClient(int serverSocket, int clientSocket);
void com() {
    printf("Communications Starting\n");
    //INDEV
}

void con() {
    printf("TELNET PORT : %d\n", PORT);
    //INDEV
}
typedef struct User {
    char* username;
    char* password;
} User;



#define MAX_USERS 10
int numUsers = 2; // Update this when adding more users
User users[MAX_USERS] = {
    {"admin", "admin"},
    {"user", "password"}
    //add more users if you need
};

int isValidUser(const char* username, const char* password, User* users, int numUsers) {
    for (int i = 0; i < numUsers; i++) {
        printf("Comparing with user: %s, password: %s\n", users[i].username, users[i].password);
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0) {
            return 1;
        }
    }
    return 0;
}

int main() {
    int serverSocket;

    initializeCommandHandler();

    registerCommand("help", executeHelpCommand);
    registerCommand("echo", executeEchoCommand);

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Bind failed");
        close(serverSocket);
        exit(EXIT_FAILURE);
    }

    if (listen(serverSocket, MAX_CLIENTS) == -1) {
        perror("Listen failed");
        close(serverSocket);
        exit(EXIT_FAILURE);
    }

    printf("Telnet Starting\n");
    con();
    com();
    cnc();

    while (1) {
        int clientSocket = accept(serverSocket, NULL, NULL);
        if (clientSocket == -1) {
            perror("Accept failed");
            close(serverSocket);
            exit(EXIT_FAILURE);
        }

        printf("Client connected\n");

        handleClient(serverSocket, clientSocket);

        close(clientSocket);

        printf("Client disconnected\n");
    }

    close(serverSocket);

    return 0;
}

void handleClient(int serverSocket, int clientSocket) {
    char buffer[BUFFER_SIZE];
    int bytesRead;

    const char* welcomeMsg = "Telnet\r\n";
    if (send(clientSocket, welcomeMsg, strlen(welcomeMsg), 0) == -1) {
        perror("Welcome message send failed");
        close(clientSocket);
        return;
    }

    const char* userPrompt = "user: ";
    if (send(clientSocket, userPrompt, strlen(userPrompt), 0) == -1) {
        perror("User prompt send failed");
        close(clientSocket);
        return;
    }

    memset(buffer, 0, sizeof(buffer));
    bytesRead = recv(clientSocket, buffer, BUFFER_SIZE, 0);
    if (bytesRead > 0) {
        buffer[bytesRead - 2] = '\0';
    }

    if (strcmp(buffer, "admin") != 0) {
        const char* errorMsg = "Invalid username Please try again.\r\n";
        send(clientSocket, errorMsg, strlen(errorMsg), 0);
        close(clientSocket);
        return;
    }

    // Send the acknowledgment for the username
    const char* usernameAckMsg = "Username received Please enter your password.\r\n";
    send(clientSocket, usernameAckMsg, strlen(usernameAckMsg), 0);

    const char* passwordPrompt = "password: ";
    if (send(clientSocket, passwordPrompt, strlen(passwordPrompt), 0) == -1) {
        perror("Password prompt send failed");
        close(clientSocket);
        return;
    }

    memset(buffer, 0, sizeof(buffer));
    bytesRead = recv(clientSocket, buffer, BUFFER_SIZE, 0);
    if (bytesRead > 0) {
        buffer[bytesRead - 2] = '\0';
    }
    char password[20];
    strcpy(password, buffer);
    printf("Received password: %s\n", password);

    if (isValidUser("admin", password, users, MAX_USERS)) {
        const char* successMsg = "Login successful. Welcome!\r\n";
        send(clientSocket, successMsg, strlen(successMsg), 0);
        const char* banner =
            "     --*Welcome To Telnet*-- ";
        send(clientSocket, banner, strlen(banner), 0);
        while (1) {
            bytesRead = recv(clientSocket, buffer, BUFFER_SIZE, 0);
            if (bytesRead <= 0) {
                break;
            }
            buffer[bytesRead - 2] = '\0';
            executeCommand(clientSocket, buffer);
        }
    } else {
        const char* errorMsg = "Invalid password. Please try again.\r\n";
        send(clientSocket, errorMsg, strlen(errorMsg), 0);
    }
    close(clientSocket);
}
