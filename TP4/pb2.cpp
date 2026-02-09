#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

volatile uint8_t gMinuterieExpiree;
volatile uint8_t gBoutonPoussoir;
void flasherLumiere();

ISR(INT0_vect)
{
    gBoutonPoussoir = 1;
    EIFR |= (1 << INTF0); // Clear INT0 flag
}
ISR(TIMER1_COMPA_vect)
{
    gMinuterieExpiree = 1;
}

void initialisation()
{
    cli();                           // Désactiver les interruptions globales
    DDRA |= (1 << PA0) | (1 << PA1); // Sorties pour les lumières
    DDRD &= ~(1 << PD2);             // Entrée pour le bouton
    EIMSK |= (1 << INT0);            // Activer INT0
    EICRA |= (1 << ISC00);           // Déclenchement INT0 sur front quelconque
    sei();                           // Activer les interruptions globales                           // Activer les interruptions globales
}
void partirMinuterie(uint16_t duree)
{
    // 80000000 / 1024 = 7812.5 ticks par seconde
    // 2^16 - 1 = 65535
    // Durée maximale = 65535 / 7812.5 = 8.39 secondes

    gMinuterieExpiree = 0;
    // mode CTC du timer 1 avec horloge divisée par 1024

    // interruption après la durée spécifiée

    TCNT1 = 0; // Remise à zéro du compteur

    OCR1A = duree; // Valeur de comparaison

    TCCR1A = 0; // Mode CTC

    TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10); // Horloge divisée par 1024

    TCCR1C = 0; // Pas utilisé

    TIMSK1 = (1 << OCIE1A); // Activer l'interruption du timer 1
}
void flasherLumiere()
{
    PORTA |= (1 << PA0);  // Allumer la lumière verte
    _delay_ms(10);        // Durée d'allumage
    PORTA &= ~(1 << PA0); // Éteindre la lumière verte
}

int main()
{
    initialisation();
    bool boutonAppuye = false;
    while (true)
    {
        _delay_ms(10000);
        gBoutonPoussoir = 0;
        flasherLumiere();
        partirMinuterie(7812); // 1 secondes
        while (!gMinuterieExpiree)
        {
            if (gBoutonPoussoir)
            {
                boutonAppuye = true;
            }
        }
        if (boutonAppuye)
        {
            boutonAppuye = false;
            PORTA |= (1 << PA1); // Allumer la lumière rouge
            PORTA &= ~(1 << PA0); // Éteindre la lumière verte
        }
        else
        {
            PORTA |= (1 << PA0); // Éteindre la lumière rouge
        }
        _delay_ms(1000);
        PORTA &= ~(1 << PA1); // Éteindre la lumière rouge
        PORTA &= ~(1 << PA0); // Éteindre la lumière verte
        
    }
}
