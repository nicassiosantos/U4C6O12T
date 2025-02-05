#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/adc.h"
#include "pico/bootrom.h"

// arquivo .pio
#include "pio_matrix.pio.h"

#define NUM_PIXELS 25


#define OUT_PIN 7 // pino do led 5x5
#define LED_R 13 // pino led vermelho
#define LED_G 11 // pino led verde
#define LED_B 12 // pino led azul 
#define button_A 5 // pino do botão A
#define button_B 6 // pino do botão B


//Variavél global
static volatile uint cont = 0;
static volatile uint32_t last_time = 0;
PIO pio = pio0; 
uint volatile sm_global = 0;
bool volatile green_on = false; 
bool volatile blue_on = false; 


// FUNÇÕES DOS NÚMEROS
extern void numeros(PIO pio, uint sm, uint cont);

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
            printf("O led verde foi desligado\n");
            green_on = false;
            gpio_put(LED_G, green_on);
        }else{
            printf("O led verde foi ligado\n");
            green_on = true;
            gpio_put(LED_G, green_on);
        }
    }else if (gpio == button_B && (current_time - last_time > 200000)) {
        printf("ENTROU\n");
        last_time = current_time;
        if(blue_on){
            printf("O led azul foi desligado\n");
            blue_on = false;
            gpio_put(LED_B, blue_on);
        }else{
            printf("O led azul foi ligado\n");
            blue_on = true;
            gpio_put(LED_B, blue_on);
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

    stdio_init_all();

    // configurações da PIO
    uint offset = pio_add_program(pio, &pio_matrix_program);
    uint sm = pio_claim_unused_sm(pio, true);
    sm_global = sm;
    pio_matrix_program_init(pio, sm, offset, OUT_PIN);

    // Configuração das interrupções com callback

    gpio_set_irq_enabled_with_callback(button_A, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

    gpio_set_irq_enabled_with_callback(button_B, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

    numeros(pio, sm, cont); //imprime o valor de cont na matriz de leds antes de entrar no while principal

    while (true)
    {
        // LED piscando 5 vezes por segundo
        gpio_put(LED_R, 1); 
        sleep_ms(100);            
        gpio_put(LED_R, 0); 
        sleep_ms(100);   
                     
    }
}