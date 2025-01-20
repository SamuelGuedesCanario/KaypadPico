Projeto Keypad com Raspberry Pi Pico
Este projeto implementa a interface com um teclado matricial (keypad) conectado a um Raspberry Pi Pico, utilizando LEDs e buzzers para respostas interativas às teclas pressionadas. Ele serve como base para atividades didáticas em projetos de microcontroladores.

Vídeo: Apresentação da solução trabalhada e os resultados obtidos nos experimentos
LINK: https://youtu.be/wzcTBoQGNF4

Objetivo
O código visa proporcionar o controle de:

LEDs que mudam de estado conforme teclas específicas (A, B, C, D) são pressionadas.
Dois buzzers que emitem sons ao pressionar uma tecla especial (#).
Um loop contínuo que detecta e exibe no terminal a tecla pressionada.
Além disso, oferece pontos de desenvolvimento para que os alunos implementem funcionalidades e completem o projeto.

Funcionalidades do Projeto
Interface com Keypad:

Detecta teclas pressionadas em um teclado matricial 4x4.
Mapeamento das teclas para diferentes funcionalidades.
Controle de LEDs:

LEDs conectados aos pinos GPIO do Raspberry Pi Pico.
Ações diferentes para as teclas A, B, C e D.
Controle de Buzzers:

Buzzers configurados para emitir sons com frequência e duração específicas ao pressionar #.
Desafio para Estudantes:

Pontos marcados com TODO no código para que os alunos adicionem lógica de controle e outras funcionalidades.
Pré-requisitos
Hardware necessário:

Raspberry Pi Pico.
Keypad matricial 4x4.
LEDs e resistores.
Dois buzzers.
Cabos de conexão.
Bibliotecas utilizadas:

Biblioteca padrão do Raspberry Pi Pico: pico/stdlib.h.
Controle de hardware e PWM: hardware/clocks.h, hardware/pwm.h.
Configuração do Projeto
Conecte os pinos do keypad aos GPIOs do Raspberry Pi Pico conforme a tabela abaixo:

Linha/Coluna	GPIO Conectado
Linha 1	GPIO 18
Linha 2	GPIO 19
Linha 3	GPIO 20
Linha 4	GPIO 4
Coluna 1	GPIO 9
Coluna 2	GPIO 8
Coluna 3	GPIO 16
Coluna 4	GPIO 17

Conecte os LEDs e buzzers aos seguintes GPIOs:

LED verde: GPIO 11.
LED azul: GPIO 12.
LED vermelho: GPIO 13.
Buzzer 1: GPIO 21.
Buzzer 2: GPIO 4.
Compile e envie o código para o Raspberry Pi Pico utilizando um ambiente de desenvolvimento como o VS Code com o Pico SDK configurado.

Estrutura do Código
O código está organizado da seguinte forma:

Definições de constantes: Mapeamento de GPIOs, configuração do keypad e frequência do buzzer.
Declaração e implementação de funções:
Inicialização do buzzer (pwm_init_buzzer).
Emissão de beep (beep).
Inicialização do keypad (keypad_init).
Leitura de teclas do keypad (keypad_get_key).
Função main:
Inicialização do terminal e do keypad.
Loop contínuo para verificar teclas pressionadas e executar ações.
Atividades Propostas
Implementar controle de LEDs:

Use o loop principal para acender LEDs específicos conforme as teclas A, B, C e D são pressionadas.
Configurar buzzers:

Acione os buzzers para emitir sons quando a tecla # for pressionada.
Desenvolver lógica de código secreto (opcional):
Como Funciona o Código Especial
O projeto inclui uma funcionalidade de código secreto, onde uma sequência específica de teclas no teclado matricial ativa uma rotina especial. Abaixo estão as instruções detalhadas sobre como o código especial funciona.

Definição do Código Especial
O código secreto é uma sequência pré-definida de teclas que o usuário deve digitar. Neste projeto, o código secreto é:

#define SECRET_CODE "1234"
Código secreto atual: 1234
Tamanho do código secreto: 4 teclas
Funcionamento Geral
Captura das Teclas:

Cada tecla pressionada no teclado matricial é capturada e exibida no terminal.
As teclas válidas para o código secreto são adicionadas a um buffer temporário (entered_code).
Verificação do Código:

Após pressionar 4 teclas (tamanho do código secreto), o buffer é comparado com o código secreto.
Se as teclas digitadas corresponderem ao código, a rotina especial é ativada.
Caso contrário, o buffer é limpo para permitir uma nova tentativa.

Rotina Especial:

Quando o código correto é detectado, os LEDs (vermelho, verde e azul) piscam 5 vezes, indicando o sucesso.
Reinicialização do Buffer:

Após a execução da rotina especial ou tentativa incorreta, o buffer é reiniciado automaticamente.
Etapas do Código
1. Captura das Teclas
As teclas pressionadas são capturadas pela função keypad_get_key() no loop principal. Exemplo:

char key = keypad_get_key();
if (key != '\0') {
    printf("Tecla pressionada: %c\n", key);
    entered_code[code_index++] = key; // Adiciona ao buffer
}
2. Verificação do Código
Quando o buffer atinge o tamanho do código secreto, ele é comparado com o código predefinido:

if (code_index == CODE_LENGTH) {
    entered_code[CODE_LENGTH] = '\0'; // Adiciona o terminador de string
    if (strcmp(entered_code, SECRET_CODE) == 0) {
        special_routine(); // Código correto
    } else {
        printf("Código incorreto: %s\n", entered_code); // Código errado
    }
    code_index = 0; // Reinicia o buffer
}
3. Rotina Especial
Quando o código correto é inserido, a função special_routine() é chamada, fazendo os LEDs piscarem:

void special_routine() {
    for (int i = 0; i < 5; i++) {
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

Exemplo de Uso:

Código Secreto Correto
Pressione as teclas na ordem correta: 1, 2, 3, 4.
Resultado:
Todos os LEDs (vermelho, verde e azul) piscarão 5 vezes.
Mensagem no terminal: Rotina especial ativada!

Código Secreto Incorreto
Pressione uma sequência diferente, como 1, 2, 5, 6.

Resultado:
Mensagem no terminal: Código incorreto: 1256
O buffer será reiniciado para aceitar uma nova tentativa.
Resumo do Fluxo
Pressione as teclas válidas (A, B, C, #) para digitar o código secreto.
O sistema captura as teclas e as armazena no buffer entered_code.
Quando o número de teclas digitadas atinge 4 (tamanho do código secreto):
Se o código for correto, a rotina especial será ativada.
Se o código for incorreto, o sistema limpa o buffer e aguarda uma nova tentativa.
Por Que Usar o Código Especial?
O código especial pode ser expandido para:

Desafios interativos.
Segurança: Simule sistemas que exigem senhas para liberar acesso.
Jogos simples: Onde sequências de teclas ativam ações específicas

Adicione uma funcionalidade onde a combinação correta de teclas ativa uma ação especial.
Observações Finais
Este código é uma base para projetos mais avançados, como sistemas de segurança, jogos simples ou interfaces interativas.
Os comentários e TODO no código foram feitos para facilitar o entendimento e a personalização do projeto pelos desenvolvedores.
Se precisar de ajustes ou mais detalhes no README, é só avisar!
