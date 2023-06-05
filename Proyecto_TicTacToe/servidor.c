#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#define PORT 8080

char board[3][3] = { {' ', ' ', ' '}, {' ', ' ', ' '}, {' ', ' ', ' '} };
int currentPlayer = 1;
int clientSockets[2] = {0};

void displayBoard() {
    printf("\n");
    printf(" %c | %c | %c \n", board[0][0], board[0][1], board[0][2]);
    printf("---+---+---\n");
    printf(" %c | %c | %c \n", board[1][0], board[1][1], board[1][2]);
    printf("---+---+---\n");
    printf(" %c | %c | %c \n", board[2][0], board[2][1], board[2][2]);
    printf("\n");
}

bool isValidMove(int row, int col) {
    if (row < 0 || row >= 3 || col < 0 || col >= 3)
        return false;
    if (board[row][col] != ' ')
        return false;
    return true;
}

bool checkWin() {
    // Comprobar filas
    for (int i = 0; i < 3; i++) {
        if (board[i][0] != ' ' && board[i][0] == board[i][1] && board[i][1] == board[i][2])
            return true;
    }

    // Comprobar columnas
    for (int j = 0; j < 3; j++) {
        if (board[0][j] != ' ' && board[0][j] == board[1][j] && board[1][j] == board[2][j])
            return true;
    }

    // Comprobar diagonales
    if (board[0][0] != ' ' && board[0][0] == board[1][1] && board[1][1] == board[2][2])
        return true;

    if (board[0][2] != ' ' && board[0][2] == board[1][1] && board[1][1] == board[2][0])
        return true;

    return false;
}

bool checkDraw() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == ' ')
                return false;
        }
    }
    return true;
}

void updateBoard(int row, int col, char symbol) {
    board[row][col] = symbol;
}

void handleClientMove(int clientSocket, int row, int col) {
    if (isValidMove(row, col)) {
        char symbol = (currentPlayer == 1) ? 'X' : 'O';
        updateBoard(row, col, symbol);
        displayBoard();

        if (checkWin()) {
            printf("Player %d wins!\n", currentPlayer);
            write(clientSocket, "win", sizeof("win"));
            write(clientSockets[(currentPlayer % 2)], "lose", sizeof("lose"));
            exit(0);
        } else if (checkDraw()) {
            printf("It's a draw!\n");
            write(clientSocket, "draw", sizeof("draw"));
            write(clientSockets[(currentPlayer % 2)], "draw", sizeof("draw"));
            exit(0);
        }

        currentPlayer = (currentPlayer % 2) + 1;
        write(clientSockets[(currentPlayer - 1)], "valid", sizeof("valid"));
        write(clientSockets[currentPlayer % 2], "wait", sizeof("wait"));
    } else {
        write(clientSocket, "invalid", sizeof("invalid"));
    }
}

int main() {
    int serverSocket, newSocket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // Crear socket
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Asociar socket a un puerto
    if (bind(serverSocket, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Escuchar nuevas conexiones
    if (listen(serverSocket, 2) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Waiting for players...\n");

    // Aceptar conexiones de dos clientes
    for (int i = 0; i < 2; i++) {
        if ((newSocket = accept(serverSocket, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
            perror("Accept failed");
            exit(EXIT_FAILURE);
        }

        printf("Player %d connected\n", i + 1);
        clientSockets[i] = newSocket;
        write(clientSockets[i], "start", sizeof("start"));
    }

    printf("Game started!\n");
    displayBoard();

    while (true) {
        char buffer[1024] = {0};
        int row, col;

        // Leer movimiento del cliente actual
        read(clientSockets[(currentPlayer - 1)], buffer, sizeof(buffer));
        sscanf(buffer, "%d,%d", &row, &col);

        // Procesar movimiento del cliente actual
        handleClientMove(clientSockets[(currentPlayer - 1)], row, col);
    }

    return 0;
}
