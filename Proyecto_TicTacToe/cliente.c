#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#define PORT 8080

void displayBoard(char board[3][3]) {
    printf("\n");
    printf(" %c | %c | %c \n", board[0][0], board[0][1], board[0][2]);
    printf("---+---+---\n");
    printf(" %c | %c | %c \n", board[1][0], board[1][1], board[1][2]);
    printf("---+---+---\n");
    printf(" %c | %c | %c \n", board[2][0], board[2][1], board[2][2]);
    printf("\n");
}

bool isValidMove(char board[3][3], int row, int col) {
    if (row < 0 || row >= 3 || col < 0 || col >= 3)
        return false;
    if (board[row][col] != ' ')
        return false;
    return true;
}

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;

    // Crear socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\nSocket creation error\n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convertir dirección IP de la forma legible a la forma binaria
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported\n");
        return -1;
    }

    // Conectar al servidor
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed\n");
        return -1;
    }

    char buffer[1024] = {0};
    char board[3][3] = { {' ', ' ', ' '}, {' ', ' ', ' '}, {' ', ' ', ' '} };
    int currentPlayer = 1;
    bool gameOver = false;

    // Esperar instrucciones del servidor para comenzar el juego
    read(sock, buffer, sizeof(buffer));

    if (strcmp(buffer, "start") == 0) {
        printf("Game started!\n");
        displayBoard(board);
    }

    while (!gameOver) {
        int row, col;
        printf("Player %d, enter your move (row,column): ", currentPlayer);
        scanf("%d,%d", &row, &col);

        // Enviar movimiento al servidor
        char move[5];
        sprintf(move, "%d,%d", row, col);
        send(sock, move, strlen(move), 0);

        // Esperar respuesta del servidor
        read(sock, buffer, sizeof(buffer));

        if (strcmp(buffer, "valid") == 0) {
            board[row][col] = (currentPlayer == 1) ? 'X' : 'O';
            displayBoard(board);
            currentPlayer = (currentPlayer % 2) + 1;
        } else if (strcmp(buffer, "invalid") == 0) {
            printf("Invalid move. Try again.\n");
        } else if (strcmp(buffer, "win") == 0) {
            printf("Congratulations! You win!\n");
            gameOver = true;
        } else if (strcmp(buffer, "lose") == 0) {
            printf("You lose! Better luck next time.\n");
            gameOver = true;
        } else if (strcmp(buffer, "draw") == 0) {
            printf("It's a draw!\n");
            gameOver = true;
        } else if (strcmp(buffer, "wait") == 0) {
            printf("Waiting for the other player's move...\n");
        }
    }

    close(sock);
    return 0;
}
