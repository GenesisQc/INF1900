#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#define MOTOR_PIN PA0
#define MOTOR_PIN2 PA2



const uint16_t delay_time = 1; //ms

void pwm_60hz(uint8_t duty)
{
    switch (duty)
    {
        case 0:
            PORTA &= ~((1 << MOTOR_PIN) | (1 << MOTOR_PIN2));

            _delay_ms(16);
            break;

        case 25:
            PORTA |= (1 << MOTOR_PIN) | (1 << MOTOR_PIN2);
            _delay_ms(4);
            
        &= ~((1 << MOTOR_PIN) | (1 << MOTOR_PIN2));
            _delay_ms(12);
            break;

        case 50:
            PORTA |= (1 << MOTOR_PIN) | (1 << MOTOR_PIN2);
            _delay_ms(8);
            PORTA &= ~((1 << MOTOR_PIN) | (1 << MOTOR_PIN2));
            _delay_ms(8);
            break;

        case 75:
            PORTA |= (1 << MOTOR_PIN) | (1 << MOTOR_PIN2);
            _delay_ms(12);
            PORTA &= ~((1 << MOTOR_PIN) | (1 << MOTOR_PIN2));
            _delay_ms(4);
            break;

        case 100:
            PORTA |= (1 << MOTOR_PIN) | (1 << MOTOR_PIN2);
            _delay_ms(16);
            break;
    }
}
void pwm_400hz(uint8_t duty)
{
    switch (duty)
    {
        case 0:
            PORTA &= ~((1 << MOTOR_PIN) | (1 << MOTOR_PIN2));
            _delay_us(2500);
            break;

        case 25:
            PORTA |= (1 << MOTOR_PIN) | (1 << MOTOR_PIN2);
            _delay_us(625);
            PORTA &= ~((1 << MOTOR_PIN) | (1 << MOTOR_PIN2));
            _delay_us(1875);
            break;

        case 50:
            PORTA |= (1 << MOTOR_PIN) | (1 << MOTOR_PIN2);
            _delay_us(1250);
            PORTA &= ~((1 << MOTOR_PIN) | (1 << MOTOR_PIN2));
            _delay_us(1250);
            break;

        case 75:
            PORTA |= (1 << MOTOR_PIN) | (1 << MOTOR_PIN2);
            _delay_us(1875);
            PORTA &= ~((1 << MOTOR_PIN) | (1 << MOTOR_PIN2));
            _delay_us(625);
            break;

        case 100:
            PORTA |= (1 << MOTOR_PIN) | (1 << MOTOR_PIN2);
            _delay_us(2500);
            break;
    }
}

int main(void)
{
    DDRA |= (1 << MOTOR_PIN) | (1 << MOTOR_PIN2); // Configure MOTOR_PIN as output

    uint8_t duties[] = {0, 25, 50, 75, 100};

    // 60 Hz
    while (1)
    {
        for (uint8_t d = 0; d < 5; d++)
        {
            for (uint16_t i = 0; i < 120; i++)
            {
                pwm_60hz(duties[d]);
            }
        }

        // 400 Hz
        for (uint8_t d = 0; d < 5; d++)
        {
            for (uint16_t i = 0; i < 800; i++)
            {
                pwm_400hz(duties[d]);
            }
        }
    }
}
