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

// pino do led 5x5
#define OUT_PIN 7

// pino do buzzer
#define BUZZER_PIN 28

extern uint32_t matrix_rgb(double b, double r, double g);

void desligar_matriz(PIO pio, uint sm){
    double frame[1][25] = {
        {0, 0, 0, 0, 0,
         0, 0,  0,  0, 0,
         0, 0,  0,  0, 0,
         0, 0,  0,  0, 0,
         0, 0, 0, 0, 0}
    };

    uint32_t valor_led;
    
    for (int16_t i = 0; i < NUM_PIXELS; i++)
    {
        valor_led = matrix_rgb(0.0, frame[0][24 - i], 0.0); // LEDs vermelhos para o número 0
        pio_sm_put_blocking(pio, sm, valor_led);
    }

}

//Função para desenhar o número 0 na matriz
void numero_0(PIO pio, uint sm){
    double frame[1][25] = {
        {0, 0.2, 0.2, 0.2, 0,
         0, 0.2,  0,  0.2, 0,
         0, 0.2,  0,  0.2, 0,
         0, 0.2,  0,  0.2, 0,
         0, 0.2, 0.2, 0.2, 0}
    };

    uint32_t valor_led;
    
    for (int16_t i = 0; i < NUM_PIXELS; i++)
    {
        valor_led = matrix_rgb(0.0, frame[0][24 - i], 0.0); // LEDs vermelhos para o número 0
        pio_sm_put_blocking(pio, sm, valor_led);
    }

}

//Função para desenhar o número 1 na matriz
void numero_1(PIO pio, uint sm){
    double frame[1][25] = {
        {0, 0  ,    0 ,  0.2, 0,
         0, 0.2,   0.2,   0 , 0,
         0, 0.2,    0 ,  0.2, 0,
         0, 0.2,    0 ,   0 , 0,
         0, 0  ,    0 ,  0.2, 0}
    };

    uint32_t valor_led;
    
    for (int16_t i = 0; i < NUM_PIXELS; i++)
    {
        valor_led = matrix_rgb(0.0, frame[0][24 - i], 0.0); // LEDs vermelhos para o número 1
        pio_sm_put_blocking(pio, sm, valor_led);
    }

}

//Função para desenhar o número 2 na matriz
void numero_2(PIO pio, uint sm){
    double frame[1][25] = {
        {0, 0.2,    0.2,  0.2, 0,
         0, 0.2,    0  ,   0 , 0,
         0, 0  ,    0.2,   0 , 0,
         0, 0  ,    0  ,  0.2, 0,
         0, 0.2,    0.2,  0.2, 0}
    };

    uint32_t valor_led;
    
    for (int16_t i = 0; i < NUM_PIXELS; i++)
    {
        valor_led = matrix_rgb(0.0, frame[0][24 - i], 0.0); // LEDs vermelhos para o número 2
        pio_sm_put_blocking(pio, sm, valor_led);
    }

}

//Função para desenhar o número 3 na matriz
void numero_3(PIO pio, uint sm){
    double frame[1][25] = {
        {0, 0.2,    0.2,  0.2, 0,
         0, 0.2,    0  ,   0 , 0,
         0, 0.2,    0.2,  0.2, 0,
         0, 0.2,      0,    0, 0,
         0, 0.2,    0.2,  0.2, 0}
    };

    uint32_t valor_led;
    
    for (int16_t i = 0; i < NUM_PIXELS; i++)
    {
        valor_led = matrix_rgb(0.0, frame[0][24 - i], 0.0); // LEDs vermelhos para o número 3
        pio_sm_put_blocking(pio, sm, valor_led);
    }

}

//Função para desenhar o número 4 na matriz
void numero_4(PIO pio, uint sm){
    double frame[1][25] = {
        {0, 0.2,    0,  0.2, 0,
         0, 0.2,    0,  0.2, 0,
         0, 0.2,    0.2,  0.2, 0,
         0, 0.2,      0,    0, 0,
         0, 0  ,    0,  0.2, 0}
    };

    uint32_t valor_led;
    
    for (int16_t i = 0; i < NUM_PIXELS; i++)
    {
        valor_led = matrix_rgb(0.0, frame[0][24 - i], 0.0); // LEDs vermelhos para o número 4
        pio_sm_put_blocking(pio, sm, valor_led);
    }

}

//Função para desenhar o número 5 na matriz
void numero_5(PIO pio, uint sm){
    double frame[1][25] = {
        {0, 0.2,    0.2,  0.2, 0,
         0, 0,    0,  0.2,   0,
         0, 0.2,    0.2,  0.2, 0,
         0, 0.2,      0,    0, 0,
         0, 0.2 ,    0.2,  0.2, 0}
    };

    uint32_t valor_led;
    
    for (int16_t i = 0; i < NUM_PIXELS; i++)
    {
        valor_led = matrix_rgb(0.0, frame[0][24 - i], 0.0); // LEDs vermelhos para o número 5
        pio_sm_put_blocking(pio, sm, valor_led);
    }

}

//Função para desenhar o número 6 na matriz
void numero_6(PIO pio, uint sm){
    double frame[1][25] = {
        {0, 0.2,    0.2,  0.2, 0,
         0, 0,    0,  0.2,   0,
         0, 0.2,    0.2,  0.2, 0,
         0, 0.2,      0,    0.2, 0,
         0, 0.2 ,    0.2,  0.2, 0}
    };

    uint32_t valor_led;
    
    for (int16_t i = 0; i < NUM_PIXELS; i++)
    {
        valor_led = matrix_rgb(0.0, frame[0][24 - i], 0.0); // LEDs vermelhos para o número 6
        pio_sm_put_blocking(pio, sm, valor_led);
    }

}

//Função para desenhar o número 7 na matriz
void numero_7(PIO pio, uint sm){
    double frame[1][25] = {
        {0, 0.2 ,    0.2 ,  0.2, 0,
         0, 0.2,   0,   0 , 0,
         0, 0,    0 ,  0.2, 0,
         0, 0.2,    0 ,   0 , 0,
         0, 0  ,    0 ,  0.2, 0}
    };

    uint32_t valor_led;
    
    for (int16_t i = 0; i < NUM_PIXELS; i++)
    {
        valor_led = matrix_rgb(0.0, frame[0][24 - i], 0.0); // LEDs vermelhos para o número 7
        pio_sm_put_blocking(pio, sm, valor_led);
    }

}

//Função para desenhar o número 8 na matriz
void numero_8(PIO pio, uint sm){
    double frame[1][25] = {
        {0, 0.2, 0.2, 0.2, 0,
         0, 0.2,  0,  0.2, 0,
         0, 0.2,  0.2,  0.2, 0,
         0, 0.2,  0,  0.2, 0,
         0, 0.2, 0.2, 0.2, 0}
    };

    uint32_t valor_led;
    
    for (int16_t i = 0; i < NUM_PIXELS; i++)
    {
        valor_led = matrix_rgb(0.0, frame[0][24 - i], 0.0); // LEDs vermelhos para o número 8
        pio_sm_put_blocking(pio, sm, valor_led);
    }

}

//Função para desenhar o número 9 na matriz
void numero_9(PIO pio, uint sm){
    double frame[1][25] = {
        {0, 0.2, 0.2, 0.2, 0,
         0, 0.2,  0,  0.2, 0,
         0, 0.2,  0.2,  0.2, 0,
         0, 0.2,  0,  0, 0,
         0, 0.2, 0.2, 0.2, 0}
    };

    uint32_t valor_led;
    
    for (int16_t i = 0; i < NUM_PIXELS; i++)
    {
        valor_led = matrix_rgb(0.0, frame[0][24 - i], 0.0); // LEDs vermelhos para o número 9
        pio_sm_put_blocking(pio, sm, valor_led);
    }

}

//Função que recebe um númro inteiro e imprime o número correspondente na matriz de leds
void numeros(PIO pio, uint sm, uint cont){ 

    if(cont == 0){
        numero_0(pio, sm);
    }else if(cont == 1) {
        numero_1(pio, sm);
    }else if(cont == 2) {
        numero_2(pio, sm);
    }else if(cont == 3) {
        numero_3(pio, sm);
    }else if(cont == 4) {
        numero_4(pio, sm);
    }else if(cont == 5) {
        numero_5(pio, sm);
    }else if(cont == 6) {
        numero_6(pio, sm);
    }else if(cont == 7) {
        numero_7(pio, sm);
    }else if(cont == 8) {
        numero_8(pio, sm);
    }else if(cont == 9) {
        numero_9(pio, sm);
    }

}
