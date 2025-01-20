#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/clocks.h"
#include "hardware/pwm.h"

// Biblioteca padrão do Raspberry Pi Pico para manipulação de GPIOs e comunicação serial

// Definição do número de linhas e colunas do keypad
#define linhas 4 // Número de linhas do keypad
#define colunas 4 // Número de colunas do keypad


#define green_pin 11
#define blue_pin 12
#define red_pin 13





// Configuração do pino do buzzer
#define BUZZER_PIN 21

// Configuração da frequência do buzzer (em Hz)
#define BUZZER_FREQUENCY 100

// Definição de uma função para inicializar o PWM no pino do buzzer
void pwm_init_buzzer(uint pin) {
    // Configurar o pino como saída de PWM
    gpio_set_function(pin, GPIO_FUNC_PWM);

    // Obter o slice do PWM associado ao pino
    uint slice_num = pwm_gpio_to_slice_num(pin);

    // Configurar o PWM com frequência desejada
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, clock_get_hz(clk_sys) / (BUZZER_FREQUENCY * 4096)); // Divisor de clock
    pwm_init(slice_num, &config, true);

    // Iniciar o PWM no nível baixo
    pwm_set_gpio_level(pin, 0);
}

// Definição de uma função para emitir um beep com duração especificada
void beep(uint pin, uint duration_ms) {
    // Obter o slice do PWM associado ao pino
    uint slice_num = pwm_gpio_to_slice_num(pin);

    // Configurar o duty cycle para 50% (ativo)
    pwm_set_gpio_level(pin, 2048);

    // Temporização
    sleep_ms(duration_ms);

    // Desativar o sinal PWM (duty cycle 0)
    pwm_set_gpio_level(pin, 0);

    // Pausa entre os beeps
    sleep_ms(100); // Pausa de 100ms
}







// Configuração dos pinos GPIO usados para as linhas e colunas do keypad
unsigned int gpio_linhas[linhas] = {4, 8, 9, 28}; // Pinos GPIO conectados às linhas do keypad
unsigned int gpio_colunas[colunas] = {16, 17, 18, 19}; // Pinos GPIO conectados às colunas do keypad

// Mapeamento das teclas do keypad em forma de matriz
const char keymap[linhas][colunas] = {
    {'1', '2', '3', 'A'}, // Primeira linha de teclas
    {'4', '5', '6', 'B'}, // Segunda linha de teclas
    {'7', '8', '9', 'C'}, // Terceira linha de teclas
    {'*', '0', '#', 'D'}  // Quarta linha de teclas
};

// Função para inicializar os GPIOs do keypad
void keypad_init() {

 
        gpio_init(BUZZER_PIN);
        gpio_set_dir(BUZZER_PIN, GPIO_OUT);
        // Inicializar o PWM no pino do buzzer
        pwm_init_buzzer(BUZZER_PIN);


        gpio_init(blue_pin);
        gpio_set_dir(blue_pin, GPIO_OUT);

        gpio_init(green_pin);
        gpio_set_dir(green_pin, GPIO_OUT);

        gpio_init(red_pin);
        gpio_set_dir(red_pin, GPIO_OUT);


    // Configuração dos pinos das linhas como saída
    for (int i = 0; i < linhas; i++) {
        gpio_init(gpio_linhas[i]);            // Inicializa o pino da linha
        gpio_set_dir(gpio_linhas[i], GPIO_OUT); // Define o pino como saída
        gpio_put(gpio_linhas[i], 1);         // Coloca a linha em estado inativo (nível lógico alto)
    }
    // Configuração dos pinos das colunas como entrada com resistor pull-up
    for (int j = 0; j < colunas; j++) {
        gpio_init(gpio_colunas[j]);           // Inicializa o pino da coluna
        gpio_set_dir(gpio_colunas[j], GPIO_IN); // Define o pino como entrada
        gpio_pull_up(gpio_colunas[j]);        // Ativa o resistor pull-up para manter o pino em nível alto quando não conectado
    }
}

// Função para verificar qual tecla foi pressionada no keypad
char keypad_get_key() {
    // Percorre cada linha
    for (int i = 0; i < linhas; i++) {
        gpio_put(gpio_linhas[i], 0); // Ativa a linha atual (nível lógico baixo)

        // Verifica cada coluna para ver se alguma tecla está pressionada
        for (int j = 0; j < colunas; j++) {
            if (gpio_get(gpio_colunas[j]) == 0) { // Se a coluna está em nível lógico baixo, uma tecla foi pressionada
                while (gpio_get(gpio_colunas[j]) == 0); // Aguarda a tecla ser liberada (evita múltiplas leituras)
                gpio_put(gpio_linhas[i], 1); // Desativa a linha atual (retorna para nível lógico alto)
                return keymap[i][j];      // Retorna o caractere correspondente à tecla pressionada
            }
        }

        gpio_put(gpio_linhas[i], 1); // Desativa a linha atual (retorna para nível lógico alto)
    }

    return '\0'; // Retorna '\0' se nenhuma tecla foi pressionada
}

// Função principal do programa
int main() {
    stdio_init_all(); // Inicializa a saída serial para depuração (printf)

    // Inicializa o keypad configurando os GPIOs
    keypad_init();

    printf("Keypad iniciado. Pressione uma tecla...\n");

    // Loop infinito para leitura das teclas
    while (1) {
        char key = keypad_get_key(); // Verifica se alguma tecla foi pressionada
        if (key != '\0') { // Se uma tecla foi pressionada
            printf("Tecla pressionada: %c\n", key); // Exibe a tecla pressionada no monitor serial
            
            if(key =='A') {
            gpio_put(green_pin, 1);
            gpio_put(blue_pin, 0);
            gpio_put(red_pin, 0);
            sleep_ms(1000);
            gpio_put(green_pin, 0);
            }
            else if(key == 'B'){
            gpio_put(blue_pin, 1);
            gpio_put(green_pin, 0);
            gpio_put(red_pin, 0); 
            sleep_ms(1000);
            gpio_put(blue_pin, 0);
            } 
            else if(key == 'C') {
            gpio_put(blue_pin, 0);
            gpio_put(green_pin, 0);
            gpio_put(red_pin, 1);
            sleep_ms(1000);
            gpio_put(red_pin, 0);
            }
             else if(key == 'D') {
            gpio_put(blue_pin, 1);
            gpio_put(green_pin, 1);
            gpio_put(red_pin, 1);
            sleep_ms(1000);
            gpio_put(blue_pin, 0);
            gpio_put(green_pin, 0);
            gpio_put(red_pin, 0);

            }
            else if (key == '#'){            
            printf("Button pressed\n");
            beep(BUZZER_PIN, 1000); // Bipe de 500ms     // Liga o buzzer
                         // Aguarda 1 segundo
            }
        }
        sleep_ms(100); // Pausa de 100ms para debounce (evita leituras repetidas causadas por oscilação)
    }

    return 0; // Este ponto nunca será alcançado
}
