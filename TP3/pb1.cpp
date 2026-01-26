#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
  



const uint16_t delay_time = 1; //ms

void PWM(uint8_t duty_cycle, uint8_t pin)
{
    for (uint8_t i = 0; i < 100; i++)
    {
        if (i < duty_cycle)
            PORTA |= (1 << pin);
        else
            PORTA &= ~(1 << pin);

        _delay_us(10);
    }
}



int main(void)
{
    DDRA |= (1 << PA0) | (1 << PA1);
    uint8_t duty_cycle = 100;
    int8_t increment = -1;
    const uint16_t cycles = (3000/100);
    uint8_t button_state = 0;
    uint8_t pin = PA0;
    while (1)
    {

        for (uint16_t i = 0; i < cycles; i++)
        {
            PWM(duty_cycle, pin);
        }

        duty_cycle += increment;

        if (duty_cycle == 100 || duty_cycle == 0)
        {
            duty_cycle = 100;
            button_state += 1;
            if (button_state % 2 == 1)
                pin = PA1;
            else
                pin = PA0;
            PORTA &= ~(1 << PA0) & ~(1 << PA1);

        }


    }
}


