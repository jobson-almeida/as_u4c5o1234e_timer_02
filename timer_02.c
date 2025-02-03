#include <stdio.h>
#include "pico/stdlib.h"

#define BUTTON_A 5

// variáveis auxiliares para deboucing
volatile uint32_t last_time = 0;
volatile uint32_t current_time = 0;

// timer de acionamento dos LEDs
bool repeating_timer_callback(struct repeating_timer *t)
{
    printf("repeating_timer_callback\n");

    return true;
}

// interrupção do botão
void gpio_button_callback(uint gpio, uint32_t events)
{

    // obtem o tempo atual desde o boot
    current_time = to_us_since_boot(get_absolute_time());
    // condicional que verifica o intervalo entre o tempo inicial do boot e o tempo atual,
    // em comparação ao tempo estimado de estabilidade do botão, uma contrapedida debouncing
    if (current_time - last_time > 250000) // 250 ms de debouncing
    {
        last_time = current_time; // atualiza o tempo da última ação no botão

        if (gpio_get(BUTTON_A) == 0) // condicional que verifica o nível lógico do botão
        {
            printf("gpio_button_callback\n");
        }
    }
    gpio_acknowledge_irq(gpio, events); // limpa a interrupção
}

int main()
{
    stdio_init_all();

    // incializa as GPIOs dos LEDs por meio de uma máscara - 0b011100000000000
    gpio_init_mask(0x07 << 11);
    gpio_set_dir_out_masked(0x07 << 11); // define as GPIOs dos LEDs como saídas, utilizando uma máscara

    // inicializa e configura o botão como entrada e resistor pull-up interno.
    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A);

    // habilita uma interrupção para indentificar quando o botão for pressionado
    gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, &gpio_button_callback);

    struct repeating_timer timer; // variável de armazenamento das informações da função de timer repetitivo
    // habilita o timer usado nos acionamentos dos LEDs.
    add_repeating_timer_ms(3000, repeating_timer_callback, NULL, &timer);

    while (true)
    {
        printf("Hello, world!\n");
        sleep_ms(1000);
    }
}
