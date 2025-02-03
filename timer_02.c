#include <stdio.h>
#include "pico/stdlib.h"

int main()
{
    stdio_init_all();

    // incializa as GPIOs dos LEDs por meio de uma máscara - 0b011100000000000
    gpio_init_mask(0x07 << 11);
    gpio_set_dir_out_masked(0x07 << 11); // define as GPIOs dos LEDs como saídas, utilizando uma máscara

    while (true)
    {
        printf("Hello, world!\n");
        sleep_ms(1000);
    }
}
