// Biblioteca padrao do Raspberry Pi Pico para manipulacao de GPIOs e comunicacao serial
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/clocks.h"
#include "hardware/pwm.h"

// DEFINICOES DE CONSTANTES
#define linhas 4  // Numero de linhas do keypad
#define colunas 4 // Numero de colunas do keypad
#define green_pin 11
#define blue_pin 12
#define red_pin 13
#define BUZZER_PINA 21
#define BUZZER_PINB 4
#define BUZZER_FREQUENCY 10000
#define SECRET_CODE "1234" // Código secreto
#define CODE_LENGTH 4      // Tamanho do código secreto

// Configuracao dos pinos GPIO para as linhas e colunas do keypad
unsigned int gpio_linhas[linhas] = {18, 19, 20, 4};  // Pinos GPIO conectados as linhas do keypad
unsigned int gpio_colunas[colunas] = {9, 8, 16, 17}; // Pinos GPIO conectados as colunas do keypad

// Mapeamento das teclas do keypad em forma de matriz
const char keymap[linhas][colunas] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}};

// ALL_LED_VAR
bool all_led_var;

// Variáveis para o código secreto
char entered_code[CODE_LENGTH + 1]; // Buffer para armazenar teclas pressionadas
int code_index = 0;                 // Índice atual no buffer

// DECLARACAO DE FUNCOES
void pwm_init_buzzer(uint pin);
void beep(uint pin, uint duration_ms);
void keypad_init();
char keypad_get_key();
void led_control(int LED, bool state, char *name);
void special_routine();

// IMPLEMENTACAO DE FUNCOES

// Funcao para inicializar o PWM no pino do buzzer
void pwm_init_buzzer(uint pin)
{
    // Configurar o pino como saida de PWM.
    // (Descricao: Configura o pino do buzzer para PWM com frequencia definida.)
}

// Funcao para emitir um beep com duracao especificada
void beep(uint pin, uint duration_ms)
{
    gpio_put(pin, 1);
    sleep_ms(duration_ms);
    gpio_put(pin, 0);
}

// Funcao para inicializar os GPIOs do keypad
void keypad_init()
{
    for (int i = 0; i < linhas; i++)
    {
        gpio_init(gpio_linhas[i]);              // Inicializa o pino da linha
        gpio_set_dir(gpio_linhas[i], GPIO_OUT); // Configura como saída
        gpio_put(gpio_linhas[i], 1);            // Define o estado inicial da linha como alta (1)
    }

    // Configura os pinos das colunas como entradas com pull-up
    for (int i = 0; i < colunas; i++)
    {
        gpio_init(gpio_colunas[i]);             // Inicializa o pino da coluna
        gpio_set_dir(gpio_colunas[i], GPIO_IN); // Configura como entrada
        gpio_pull_up(gpio_colunas[i]);          // Habilita o resistor de pull-up
    }
}

// Funcao para verificar qual tecla foi pressionada no keypad
char keypad_get_key()
{
    for (int l = 0; l < linhas; l++)
    {
        gpio_put(gpio_linhas[l], 0);
        for (int c = 0; c < colunas; c++)
        {
            if (gpio_get(gpio_colunas[c]) == 0)
            {
                sleep_ms(50); // Debounce
                gpio_put(gpio_linhas[l], 1);
                return keymap[l][c];
            }
        }
        gpio_put(gpio_linhas[l], 1);
    }
    return '\0';
}

// Funcao para controle dos LEDs
void led_control(int LED, bool state, char *name)
{
    if (gpio_get(LED) != state)
        gpio_put(LED, state);
    printf("LED %d (%s): %s\n", LED, name, state ? "Ligado" : "Desligado");
}

// Funcao para a rotina especial
void special_routine()
{
    for (int i = 0; i < 5; i++) // Pisca os LEDs 5 vezes
    {
        gpio_put(red_pin, 1);
        gpio_put(green_pin, 1);
        gpio_put(blue_pin, 1);
        sleep_ms(200);
        gpio_put(red_pin, 0);
        gpio_put(green_pin, 0);
        gpio_put(blue_pin, 0);
        sleep_ms(200);
    }
    printf("Rotina especial ativada!\n");
}

int main()
{
    stdio_init_all();
    all_led_var = false;

    // Inicializa o keypad
    keypad_init();

    // Configura os pinos dos LEDs
    gpio_init(red_pin);
    gpio_set_dir(red_pin, GPIO_OUT);
    gpio_put(red_pin, 0);

    gpio_init(green_pin);
    gpio_set_dir(green_pin, GPIO_OUT);
    gpio_put(green_pin, 0);

    gpio_init(blue_pin);
    gpio_set_dir(blue_pin, GPIO_OUT);
    gpio_put(blue_pin, 0);

    // Configura o pino do buzzer
    gpio_init(BUZZER_PINA);
    gpio_set_dir(BUZZER_PINA, GPIO_OUT);
    gpio_put(BUZZER_PINA, 0);

    while (1)
    {
        char key = keypad_get_key(); // Obtem a tecla pressionada
        if (key != '\0')
        {
            printf("Tecla pressionada: %c\n", key);

            // Adiciona a tecla pressionada ao buffer
            entered_code[code_index] = key;
            code_index++;

            // Verifica se o buffer está cheio
            if (code_index == CODE_LENGTH)
            {
                entered_code[CODE_LENGTH] = '\0'; // Adiciona o terminador de string

                // Compara com o código secreto
                if (strcmp(entered_code, SECRET_CODE) == 0)
                {
                    special_routine(); // Chama a rotina especial
                }
                else
                {
                    printf("Código incorreto: %s\n", entered_code);
                }

                // Reinicia o buffer
                code_index = 0;
            }

            // Lógica para controle de LEDs e buzzer
            switch (key)
            {
            case 'A':
                led_control(red_pin, !gpio_get(red_pin), "RED");
                break;
            case 'B':
                led_control(green_pin, !gpio_get(green_pin), "GREEN");
                break;
            case 'C':
                led_control(blue_pin, !gpio_get(blue_pin), "BLUE");
                break;
            case '#':
                beep(BUZZER_PINA, 200);
                break;
            default:
                break;
            }
        }
    }

    return 0;
}
