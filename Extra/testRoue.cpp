#include <avr/io.h>
#define F_CPU 8000000
#include <util/delay.h>

int main()
{
    DDRA = 0xFF; // Configure le port A en sortie
    PORTA = 0x00; // Initialise le port A à 0x00
    const int delayTime = 10000; // Temps de délai en millisecondes
    while (1)
    {
        PORTA = 0b10-10-10-01; // Allume toutes les LED vert ici 10 = vert next 10 = tourne vert l'avant
        _delay_ms(delayTime); // Attendre
        PORTA = 0b00-10-10-10; // Éteint toutes les LED rouge ici 01 = rouge next 01 = tourne pas l'avant
        _delay_ms(delayTime); // Attendre
    }

}