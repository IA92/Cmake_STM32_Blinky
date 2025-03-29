#include "system_config.h"
#include "stm32f401xe.h"

int main(void)
{
    system_clock_init(false, false);

    /*Enable GPIO Port Clock*/
    RCC->AHB1ENR |= 1UL << 0;

    GPIO_TypeDef *p_blinky_port = GPIOA;
    uint32_t blinky_pin = 5;
    uint32_t blinky_moder_pos = 2 * blinky_pin;
    p_blinky_port->MODER = 1UL << blinky_moder_pos; 

    while (1)
    {
        uint32_t blinky_br_pos_offset = 16;
        if (p_blinky_port->ODR & (1UL << blinky_pin))
        {
            p_blinky_port->BSRR |= 1UL << (blinky_pin + blinky_br_pos_offset);
        }
        else
        {
            p_blinky_port->BSRR |= 1UL << (blinky_pin);
        }

        for (uint32_t idx = 0; idx < 100000; idx++)
        {}
    }
}