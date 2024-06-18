#include <curses.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "EasyPio"
#include "assembly.h"

// Constantes
#define DELAY_INTERVAL 250
#define DEFAULT_DELAY 1000
#define NUM_LEDS 8
#define NUM_PATTERNS 14

unsigned int QUIT;
size_t DELAY;
size_t DELAY_1 = DEFAULT_DELAY, DELAY_2 = DEFAULT_DELAY, DELAY_3 = DEFAULT_DELAY, DELAY_4 = DEFAULT_DELAY, DELAY_5 = DEFAULT_DELAY;

void delay(size_t a);

void moveCursorToOrigin();

void clear();

void displayBinary(unsigned char DISPLAY, unsigned int option);

void configurarPines(void);

void mostrarLuces(unsigned char output);

void apagarLeds();

void *keyListener() {
    while (!QUIT) {
        int key = getch();
        if (key == "q")
            QUIT = 1;
        else if (key == KEY_UP) {
            if (DELAY - DELAY_INTERVAL != 0)
                DELAY -= DELAY_INTERVAL;
        } else if (key == KEY_DOWN) {
            DELAY += DELAY_INTERVAL;
        }
    }
}

void *autoFantastico() {
    clear();
    while (!QUIT) {
        unsigned char DISPLAY = 0x80;
        for (int i = 0; i < 7; i++) {
            if (QUIT)
                break;
            displayBinary(DISPLAY, 1);
            DISPLAY = DISPLAY >> 1;
            delay(DELAY);
        }
        for (int i = 0; i < 7; i++) {
            if (QUIT)
                break;
            displayBinary(DISPLAY, 1);
            DISPLAY = DISPLAY << 1;
            delay(DELAY);
        }
    }
}

void *choque() {
    clear();
    while (!QUIT) {
        unsigned char SUB_DISPLAY_1 = 0x80;
        unsigned char SUB_DISPLAY_2 = 0x01;
        unsigned char DISPLAY = 0;

        for (int i = 0; i < 7; i++) {
            DISPLAY = SUB_DISPLAY_1 + SUB_DISPLAY_2;
            if (QUIT)
                break;
            displayBinary(DISPLAY, 2);
            SUB_DISPLAY_1 = SUB_DISPLAY_1 >> 1;
            SUB_DISPLAY_2 = SUB_DISPLAY_2 << 1;
            delay(DELAY);
        }
    }
}

void *rebote() {
    clear();
    while (!QUIT) {
        unsigned char DISPLAY = 0x80;

        for (int r = 7; r > 0; r--) {
            for (DISPLAY; DISPLAY != 0b1; DISPLAY = DISPLAY >> 1) {
                if (QUIT)
                    break;
                displayBinary(DISPLAY, 3);
                delay(DELAY);
            }
            for (int j = 0; j < r - 1; j++) {
                if (QUIT)
                    break;
                displayBinary(DISPLAY, 3);
                DISPLAY = DISPLAY << 1;
                delay(DELAY);
            }
        }
        if (QUIT)
            break;
        displayBinary(DISPLAY, 3);
        delay(DELAY);
    }
}

void *chargeBar() {
    clear();
    while (!QUIT) {
        printf("Mostrando Barra de Carga\n");
        unsigned char led = 0x80;
        for (int i = 0; i < NUM_LEDS - 1; i++) {
            if (QUIT)
                break;
            displayBinary(led, 4);
            sleep(1);
            led |= 0x80 >> (i + 1);
        }
        if (QUIT)
            break;
        displayBinary(led, 4);
        sleep(1);
    }
}

void disp_binary(unsigned char pattern) {
    for (int t = 128; t > 0; t >>= 1) {
        if (pattern & t) {
            printf("*");
        } else {
            printf("-");
        }
    }
    printf("\n");
}

void *pendulo() {
    clear();
    while (!QUIT) {
        printf("Mostrando pendulo\n");
        unsigned char patrones[NUM_PATTERNS] = {
            0x88, 0x48, 0x28, 0x18, 0x14, 0x12, 0x11,
            0x11, 0x12, 0x14, 0x18, 0x28, 0x48, 0x88
        };

        for (int i = 0; i < NUM_PATTERNS; i++) {
            if (QUIT)
                break;
            displayBinary(patrones[i], 5);
            sleep(1);
        }
    }
}

void app() {
    unsigned char option[1];
    clear();

    int i = 0;
    for (i; i < 3; i++) {
        delay(2000);
        clear();
    }

    if (i == 3)
        exit(0);

    do {
        delay(2000);
        displayBinary(0, 0);
        clear();
        QUIT = 0;

        printf("------ M E N U ------\n\r");
        printf("1. Auto Fantastico\n\r");
        printf("2. Choque\n\r");
        printf("3. Rebote\n\r");
        printf("4. Barra de Carga\n\r");
        printf("5. Pendulo\n\r");
        printf("0. Salir\n\r");
        printf("\n\r");
        printf("\n\r---> ");

        option[0] = getchar();

        if (option[1] != '\0') {
            option[0] = 'i';
        }

        printf("\033[?25l");

        pthread_t threads[2];

        switch (option[0]) {

            case '1':
                DELAY = DELAY_1;
                pthread_create(&threads[0], NULL, keyListener, NULL);
                pthread_create(&threads[1], NULL, autoFantastico, NULL);
                pthread_join(threads[0], NULL);
                pthread_join(threads[1], NULL);
                DELAY_1 = DELAY;

                break;
            case '2':
                DELAY = DELAY_2;
                pthread_create(&threads[0], NULL, keyListener, NULL);
                pthread_create(&threads[1], NULL, choque, NULL);
                pthread_join(threads[0], NULL);
                pthread_join(threads[1], NULL);
                DELAY_2 = DELAY;

                break;
            case '3':
                DELAY = DELAY_3;
                pthread_create(&threads[0], NULL, keyListener, NULL);
                pthread_create(&threads[1], NULL, rebote, NULL);
                pthread_join(threads[0], NULL);
                pthread_join(threads[1], NULL);
                DELAY_3 = DELAY;

                break;
            case '4':
                DELAY = DELAY_4;
                pthread_create(&threads[0], NULL, keyListener, NULL);
                pthread_create(&threads[1], NULL, chargeBar, NULL);
                pthread_join(threads[0], NULL);
                pthread_join(threads[1], NULL);
                DELAY_4 = DELAY;

                break;
            case '5':
                DELAY = DELAY_5;
                pthread_create(&threads[0], NULL, keyListener, NULL);
                pthread_create(&threads[1], NULL, pendulo, NULL);
                pthread_join(threads[0], NULL);
                pthread_join(threads[1], NULL);
                DELAY_5 = DELAY;

                break;
            case '0':
                printf("\n\n\rSaliendo del programa...\n\r");
                delay(2000);
                clear();
                exit(0);
                break;
            default:
                printf("\n\n\rIngrese una opcion valida\n\r");
                break;
        }
    } while (1);
}

int main() {
    initscr();             // Inicializar la pantalla de ncurses
    keypad(stdscr, TRUE);  // Habilitar la entrada de teclado especial
    nodelay(stdscr, TRUE); // Configurar getch() para no bloquear
    noecho();
    cbreak();

    app();

    endwin();
}

// Desarrollo de funciones

void delay(size_t a) {
    a = a * 100000;
    while (a--);
}

void moveCursorToOrigin() {
    unsigned int i = 10;
    while (i--)
        printf("\033[F");
}

void clear() {
    printf("\033[2J");
    moveCursorToOrigin();
}

void displayBinary(unsigned char DISPLAY, unsigned int option) {
    char display[8];
    int i = 0;
    for (unsigned int POINTER = 0x80; POINTER > 0; POINTER = POINTER >> 1) {
        if (POINTER & DISPLAY) {
            display[i] = '*';
            i++;
        } else {
            display[i] = '_';
            i++;
        }
    }

    switch (option) {
        case 1:
            printf("\033[1;31mSECUENCIA:\033[0m \033[1;36mAuto Fantastico\033[0m\n\r\n\r");
            break;
        case 2:
            printf("\033[1;31mSECUENCIA:\033[0m \033[1;36mChoque\033[0m\n\r\n\r");
            break;
        case 3:
            printf("\033[1;31mSECUENCIA:\033[0m \033[1;36mRebote\033[0m\n\r\n\r");
            break;
        case 4:
            printf("\033[1;31mSECUENCIA:\033[0m \033[1;36mBarra de Carga\033[0m\n\r\n\r");
            break;
        case 5:
            printf("\033[1;31mSECUENCIA:\033[0m \033[1;36mPendulo\033[0m\n\r\n\r");
            break;
        case 0:
            break;
    }

    printf("%s\n\n\r\033[1;33mDELAY: \033[0m%d   ", display, DELAY);

    if (option) {
        printf("\n\r\n\r");
        printf("\033[1;30mPresione la tecla Q para salir\033[0m");
    }

    moveCursorToOrigin();
}

void configurarPines(void) {
    pioInit(); // inicializa para usar EasyPIO
    for (int i = 0; i < 8; i++) {
        pinMode(led[i], OUTPUT); // Define si es entrada o salida
    }// Van a ser los 8 salidas

}

void mostrarLuces(unsigned char output) {
    for (int j = 0; j < 8; j++) {
        digitalWrite(led[j], (output >> j) & 1);// Escribe a un puerto
    }
}

void apagarLeds() {
    unsigned char apagado = 0x00; //es un hexa de 0000 0000
    mostrarLuces(apagado);

}