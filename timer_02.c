#include <stdio.h>
#include "pico/stdlib.h"

#define BUTTON_A 5

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

    while (true)
    {
        printf("Hello, world!\n");
        sleep_ms(1000);
    }
}
