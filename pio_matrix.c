#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/adc.h"
#include "pico/bootrom.h"
#include "hardware/i2c.h"
#include "inc/ssd1306.h"
#include "inc/font.h"

#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C

// Arquivo .pio contendo código para controle da matriz LED
#include "pio_matrix.pio.h"

#define NUM_PIXELS 25  // Número de LEDs na matriz

// Definição dos pinos utilizados
#define OUT_PIN 7   // Pino de saída para a matriz de LEDs 5x5
#define LED_R 13    // Pino do LED vermelho
#define LED_G 11    // Pino do LED verde
#define LED_B 12    // Pino do LED azul 
#define button_A 5  // Pino do botão A
#define button_B 6  // Pino do botão B

// Variáveis globais
static volatile uint32_t last_time = 0;  // Variável para controle de debounce do botão
PIO pio = pio0;  // Instância da PIO utilizada
uint volatile sm_global = 0;  // Variável global para armazenar o state machine
bool volatile green_on = false;  // Estado do LED verde
bool volatile blue_on = false;   // Estado do LED azul

ssd1306_t ssd;  // Estrutura para manipulação do display OLED

// Funções externas para controle da matriz LED
extern void numeros(PIO pio, uint sm, uint cont);
extern void desligar_matriz(PIO pio, uint sm);

// Protótipo da função de interrupção dos botões
static void gpio_irq_handler(uint gpio, uint32_t events);

/**
 * Função para definir a intensidade das cores RGB do LED.
 * 
 * @param b Intensidade do azul (0 a 1)
 * @param r Intensidade do vermelho (0 a 1)
 * @param g Intensidade do verde (0 a 1)
 * @return Valor RGB formatado para o LED
 */
uint32_t matrix_rgb(double b, double r, double g)
{
    unsigned char R, G, B;
    R = r * 255;
    G = g * 255;
    B = b * 255;
    return (G << 24) | (R << 16) | (B << 8);
}

/**
 * Função de interrupção para lidar com o pressionamento dos botões.
 * Liga ou desliga os LEDs e atualiza o display OLED com o estado dos LEDs.
 * 
 * @param gpio Pino do botão pressionado
 * @param events Evento de interrupção
 */
void gpio_irq_handler(uint gpio, uint32_t events)
{
    uint32_t current_time = to_us_since_boot(get_absolute_time());  // Obtém o tempo atual em microssegundos

    // Verifica se o botão A foi pressionado e aplica um debounce de 200ms
    if (gpio == button_A && (current_time - last_time > 200000)) {
        last_time = current_time;
        
        if (green_on) {
            printf("Após o pressionamento do botão A, o Led verde foi desligado!\n");
            green_on = false;
        } else {
            printf("Após o pressionamento do botão A, o Led verde foi ligado!\n");
            green_on = true;
        }

        gpio_put(LED_G, green_on);  // Atualiza o estado do LED verde
        ssd1306_fill(&ssd, false);  // Limpa o display
        ssd1306_draw_string(&ssd, "O Led verde", 20, 20);
        ssd1306_draw_string(&ssd, green_on ? "foi ligado" : "foi desligado", 20, 29);
        ssd1306_send_data(&ssd);  // Atualiza o display OLED
    }
    
    // Verifica se o botão B foi pressionado e aplica um debounce de 200ms
    else if (gpio == button_B && (current_time - last_time > 200000)) {
        last_time = current_time;

        if (blue_on) {
            printf("Após o pressionamento do botão B, o Led azul foi desligado!\n");
            blue_on = false;
        } else {
            printf("Após o pressionamento do botão B, o Led azul foi ligado!\n");
            blue_on = true;
        }

        gpio_put(LED_B, blue_on);  // Atualiza o estado do LED azul
        ssd1306_fill(&ssd, false);  // Limpa o display
        ssd1306_draw_string(&ssd, "O Led azul", 20, 20);
        ssd1306_draw_string(&ssd, blue_on ? "foi ligado" : "foi desligado", 20, 29);
        ssd1306_send_data(&ssd);  // Atualiza o display OLED
    }
}

/**
 * Função principal do programa.
 * Inicializa os pinos, configura os periféricos e entra em loop aguardando comandos do usuário.
 */
int main()
{
    // Inicializa os LEDs e os botões
    gpio_init(LED_G);
    gpio_set_dir(LED_G, GPIO_OUT);
    gpio_put(LED_G, green_on);

    gpio_init(LED_B);
    gpio_set_dir(LED_B, GPIO_OUT);
    gpio_put(LED_B, blue_on);

    gpio_init(button_A);
    gpio_set_dir(button_A, GPIO_IN);
    gpio_pull_up(button_A);

    gpio_init(button_B);
    gpio_set_dir(button_B, GPIO_IN);
    gpio_pull_up(button_B);

    // Inicializa a comunicação I2C para o display OLED
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT);
    ssd1306_config(&ssd);
    ssd1306_send_data(&ssd);

    // Limpa o display OLED
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);

    stdio_init_all();  // Inicializa o console USB

    // Configuração da PIO para a matriz de LEDs
    uint offset = pio_add_program(pio, &pio_matrix_program);
    uint sm = pio_claim_unused_sm(pio, true);
    sm_global = sm;
    pio_matrix_program_init(pio, sm, offset, OUT_PIN);

    // Configuração das interrupções dos botões
    gpio_set_irq_enabled_with_callback(button_A, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(button_B, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

    desligar_matriz(pio, sm);  // Desliga a matriz de LEDs

    while (true)
    {
        if (stdio_usb_connected())  // Verifica se há conexão USB ativa
        {
            char c;
            if (scanf("%c", &c) == 1) {
                if (c >= '0' && c <= '9') {  // Se o caractere for um número de 0 a 9
                    uint num = (uint)(c - '0');
                    ssd1306_fill(&ssd, false);
                    ssd1306_draw_char(&ssd, c, 20, 20);
                    ssd1306_send_data(&ssd);
                    printf("Você enviou: %c\n", c);
                    numeros(pio, sm, num);  // Exibe o número na matriz de LEDs
                } else {  // Caso seja outro caractere
                    ssd1306_fill(&ssd, false);
                    ssd1306_draw_char(&ssd, c, 20, 20);
                    ssd1306_send_data(&ssd);
                    printf("Você enviou: %c\n", c);
                }
            }
        }
        sleep_ms(10);  // Pequeno atraso para evitar sobrecarga do loop
    }
}
