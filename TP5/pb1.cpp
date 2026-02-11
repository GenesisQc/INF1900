#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "memoire_24.h"
#include <string.h>
Memoire24CXXX memoire = Memoire24CXXX();
void init()
{
    PORTA = 0xFF;
    DDRA = 0xFF;
}
int main()
{
    init();
    char message[] = "*P*O*L*Y*T*E*C*H*N*I*Q*U*E* *M*O*N*T*R*E*A*L*";
    uint16_t addresse = 0x0000;
    uint8_t longeur = strlen(message);
    char messagerecu[longeur];
    memoire.ecriture(addresse, (uint8_t *)message, longeur);
    _delay_ms(5);
    memoire.lecture(addresse, (uint8_t *)messagerecu, longeur);
    for (int i = 0; i < longeur; i++)
    {
        if (message[i] == messagerecu[i])
        {
            PORTA |= (1 << PA0);
            PORTA &= ~(1 << PA1);
        }
        else
        {
            PORTA &= ~(1 << PA1);
            PORTA &= ~(1 << PA0);
            break;
        }
    }
}