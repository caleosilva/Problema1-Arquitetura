#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/input.h>
#include <unistd.h>

#define MOUSEFILE "/dev/input/mice"

// Enumeração para as direções do mouse
enum MouseDirection {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    NONE
};

// Enumeração para os botões do mouse
enum MouseButton {
    LEFT_BUTTON = 0x01,
    RIGHT_BUTTON = 0x02,
    MIDDLE_BUTTON = 0x04
};

// Função para imprimir a direção do mouse
void printMouseDirection(int dx, int dy) {
    if (dx > 0)
        printf("Direita\n");
    else if (dx < 0)
        printf("Esquerda\n");

    if (dy > 0)
        printf("Cima\n");
    else if (dy < 0)
        printf("Baixo\n");
}

// Função para mapear eventos do mouse para direções
enum MouseDirection mapMouseDirection(int dx, int dy) {
    if (dx > 0)
        return RIGHT;
    else if (dx < 0)
        return LEFT;

    if (dy > 0)
        return UP;
    else if (dy < 0)
        return DOWN;

    return NONE;
}

int main() {
    int fd;
    char *buffer = (char*)malloc(3*sizeof(char));
    if(buffer == NULL) {
        perror("Erro de alocação de memória");
        exit(EXIT_FAILURE);
    }

    // Abra o dispositivo de entrada do mouse
    if ((fd = open(MOUSEFILE, O_RDONLY)) == -1) {
        perror("Erro ao abrir dispositivo de entrada do mouse");
        exit(EXIT_FAILURE);
    }

    printf("Lendo eventos do mouse...\n");

    while (1) {
        // Leia os eventos do mouse
        if (read(fd, buffer, 3) != 3) {
            perror("Erro ao ler evento do mouse");
            exit(EXIT_FAILURE);
        }

        // Extrai dados do buffer
        int dx = buffer[1];
        int dy = buffer[2];
        int button = buffer[0];

        // Mapeie a direção do mouse
        enum MouseDirection direction = mapMouseDirection(dx, dy);

        // Imprima a direção do mouse
        printf("Direção do mouse: ");
        switch (direction) {
            case UP:
                printf("Para cima\n");
                break;
            case DOWN:
                printf("Para baixo\n");
                break;
            case LEFT:
                printf("Para esquerda\n");
                break;
            case RIGHT:
                printf("Para direita\n");
                break;
            default:
                printf("Nenhuma direção\n");
                break;
        }

        // Verifique se o botão esquerdo do mouse foi pressionado
        if (button & LEFT_BUTTON) {
            printf("Botão esquerdo do mouse pressionado.\n");
        }

        // Verifique se o botão direito do mouse foi pressionado
        if (button & RIGHT_BUTTON) {
            printf("Botão direito do mouse pressionado.\n");
        }

        // Verifique se o botão do meio do mouse foi pressionado
        if (button & MIDDLE_BUTTON) {
            printf("Botão do meio do mouse pressionado.\n");
        }
    }

    // Nunca deve ser alcançado
    close(fd);
    free(buffer);
    return 0;
}
