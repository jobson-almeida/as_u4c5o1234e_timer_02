#include <stdio.h>
#include "pico/stdlib.h"

#define BUTTON_A 5
#define FAST_DELAY_MS 100
#define SLOW_DELAY_MS 3000

volatile uint32_t last_time = 0;    // variáveis auxiliares para deboucing
volatile uint32_t current_time = 0; // variáveis auxiliares para deboucing
volatile size_t index_led = 0; // variável para interação de acionamento dos LEDs
volatile bool button_pressed = false;  // status de botão pressionado
volatile bool change_delay_ms = false; // status para alteração do delay_ms da função do timer

struct repeating_timer timer; // variável de armazenamento das informações da função de timer repetitivo

uint8_t onoff_led_mask[3] = {0x07, 0x06, 0x04}; // máscadas de acionamento dos LEDs - 0111 | 0110 | 0100

// declaração do protótipo da função
bool repeating_timer_callback(struct repeating_timer *t);

// alarme
int64_t turn_off_callback(alarm_id_t id, void *user_data)
{
       gpio_put_masked((0x07 << 11), (0x00 << 11)); // máscara que aplica o nível lógico 0 aos três LEDs

    // DEBUGGING
    current_time = to_us_since_boot(get_absolute_time());
    printf("%d\n", current_time - last_time);

    button_pressed = false; // atualiza a variável que representado o status de pressionamento do botão 
    index_led = 0;          // atualiza varíavel de indexes das maáscaras de acionamento dos LEDs

    cancel_repeating_timer(&timer);                                                // cancela o timer atual da função 'add_repeating_timer_ms'
    add_repeating_timer_ms(FAST_DELAY_MS, repeating_timer_callback, NULL, &timer); // atualiza o timer da função 'add_repeating_timer_ms' para um novo pressionamento do botão

    return 0; // finaliza o alarme
}

// timer de acionamento dos LEDs
bool repeating_timer_callback(struct repeating_timer *t)
{
    printf("repeating_timer_callback\n");

    // condicional de confirmação de detecção de pressionamento do botão e alteração do delay de acionamento dos LEDs
    if (button_pressed && change_delay_ms)
    {
        change_delay_ms = false;
        // cancela o timer atual de detecção do botão em preparação a troca do valor delay_ms
        cancel_repeating_timer(&timer);
        // aplica o delay adequado para a interação e acionamento dos LEDs.
        // utilizar uma combinação de valores permite uma fluidez e sincronia ao primeiro acionamento
        add_repeating_timer_ms(SLOW_DELAY_MS, repeating_timer_callback, NULL, &timer);
        add_alarm_in_ms(9000, turn_off_callback, NULL, false); // adiciona um alarme com delay de acionamento de 9 segundos
    }

    if (button_pressed && index_led < 3) // condicional de confirmação de pressionamento do botão e interação do loop
    {

        // aplica a máscara de acionamento dos LEDs de acordo o indexes correspondentes
        gpio_put_masked((0x07 << 11), (onoff_led_mask[index_led] << 11));

        index_led++;
    } 

    return true;
}

// interrupção do botão
void gpio_button_callback(uint gpio, uint32_t events)
{

    // obtem o tempo atual desde o boot
    current_time = to_us_since_boot(get_absolute_time());
    // condicional que verifica o intervalo entre o tempo inicial do boot e o tempo atual,
    // em comparação ao tempo estimado de estabilidade do botão, uma contrapedida debouncing
    if (current_time - last_time > 200000) // 200 ms de debouncing
    {
        last_time = current_time; // atualiza o tempo da última ação no botão

        if (gpio_get(BUTTON_A) == 0) // condicional que verifica o nível lógico do botão
        {
            button_pressed = true;
            change_delay_ms = true;
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

    // habilita o timer usado nos acionamentos dos LEDs.
    add_repeating_timer_ms(FAST_DELAY_MS, repeating_timer_callback, NULL, &timer);

    while (true)
    {
        printf("Hello, world!\n");
        sleep_ms(1000);
    }
}
