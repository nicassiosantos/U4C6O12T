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


// arquivo .pio
#include "pio_matrix.pio.h"

#define NUM_PIXELS 25


#define OUT_PIN 7 // pino do led 5x5
#define LED_R 13 // pino led vermelho
#define LED_G 11 // pino led verde
#define LED_B 12 // pino led azul 
#define button_A 5 // pino do botão A
#define button_B 6 // pino do botão B
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C

//Variavél global
static volatile uint32_t last_time = 0;
PIO pio = pio0; 
uint volatile sm_global = 0;
bool volatile green_on = false; 
bool volatile blue_on = false; 

ssd1306_t ssd; // Inicializa a estrutura do display

// FUNÇÕES DOS NÚMEROS
extern void numeros(PIO pio, uint sm, uint cont);
extern void desligar_matriz(PIO pio, uint sm);

//Definindo o escopo da função de interrupção
static void gpio_irq_handler(uint gpio, uint32_t events);

// rotina para definição da intensidade de cores do led
uint32_t matrix_rgb(double b, double r, double g)
{
    unsigned char R, G, B;
    R = r * 255;
    G = g * 255;
    B = b * 255;
    return (G << 24) | (R << 16) | (B << 8);
}

/// Função de interrupção 
void gpio_irq_handler(uint gpio, uint32_t events)
{
    // Obtém o tempo atual em microssegundos
    uint32_t current_time = to_us_since_boot(get_absolute_time());
    

    // Verifica qual botão foi pressionado
    if (gpio == button_A && (current_time - last_time > 200000)) {
        last_time = current_time;
        if(green_on){
            printf("Após o pressionamento do botão A o led verde foi desligado!!\n");
            green_on = false;
            gpio_put(LED_G, green_on); 
            ssd1306_fill(&ssd, false); // Limpa o display 
            ssd1306_draw_string(&ssd, "O led verde", 20, 20);
            ssd1306_draw_string(&ssd, "foi desligado", 20, 29);
            ssd1306_send_data(&ssd);
        }else{
            printf("Após o pressionamento do botão A o led verde foi ligado!!\n");
            green_on = true;
            gpio_put(LED_G, green_on);
            ssd1306_fill(&ssd, false); // Limpa o display 
            ssd1306_draw_string(&ssd, "O led verde", 20, 20);
            ssd1306_draw_string(&ssd, "foi ligado", 20, 29);
            ssd1306_send_data(&ssd);
        }
    }else if (gpio == button_B && (current_time - last_time > 200000)) {
        last_time = current_time;
        if(blue_on){
            printf("Após o pressionamento do botão B, o led azul foi desligado!!\n");
            blue_on = false;
            gpio_put(LED_B, blue_on);
            ssd1306_fill(&ssd, false); // Limpa o display 
            ssd1306_draw_string(&ssd, "O led azul", 20, 20);
            ssd1306_draw_string(&ssd, "foi desligado", 20, 29);
            ssd1306_send_data(&ssd);
            
        }else{
            printf("Após o pressionamento do botão B, o led azul foi ligado!!\n");
            blue_on = true;
            gpio_put(LED_B, blue_on);
            ssd1306_fill(&ssd, false); // Limpa o display 
            ssd1306_draw_string(&ssd, "O led azul", 20, 20);
            ssd1306_draw_string(&ssd, "foi ligado", 20, 29);
            ssd1306_send_data(&ssd);
            
        }
    }
}

int main()
{
    //Inicialização dos pinos       

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


    // I2C Initialisation. Using it at 400Khz.
    i2c_init(I2C_PORT, 400 * 1000);

    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C); // Set the GPIO pin function to I2C
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C); // Set the GPIO pin function to I2C
    gpio_pull_up(I2C_SDA); // Pull up the data line
    gpio_pull_up(I2C_SCL); // Pull up the clock line
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT); // Inicializa o display
    ssd1306_config(&ssd); // Configura o display
    ssd1306_send_data(&ssd); // Envia os dados para o display

    // Limpa o display. O display inicia com todos os pixels apagados.
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);

    bool cor = true;

    


    stdio_init_all();


    // configurações da PIO
    uint offset = pio_add_program(pio, &pio_matrix_program);
    uint sm = pio_claim_unused_sm(pio, true);
    sm_global = sm;
    pio_matrix_program_init(pio, sm, offset, OUT_PIN);

    // Configuração das interrupções com callback

    gpio_set_irq_enabled_with_callback(button_A, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

    gpio_set_irq_enabled_with_callback(button_B, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

    desligar_matriz(pio, sm);

    while (true)
    {
        if (stdio_usb_connected())
        {
            char c;
            if (scanf("%c", &c) == 1){
                if(c >= '0' && c <= '9'){
                    uint num = (uint)(c - '0');
                    ssd1306_fill(&ssd, false);
                    ssd1306_draw_char(&ssd, c, 20, 20); 
                    ssd1306_send_data(&ssd);
                    printf("Você enviou:%c\n", c);
                    numeros(pio, sm, num);
                }else{
                    ssd1306_fill(&ssd, false);
                    ssd1306_draw_char(&ssd, c, 20, 20); 
                    ssd1306_send_data(&ssd);
                    printf("Você enviou:%c\n", c);
                }
            }
        
        sleep_ms(10);   
        }           
    }
}