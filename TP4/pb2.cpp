#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

volatile uint8_t gMinuterieExpiree;
volatile uint8_t gBoutonPoussoir;

void initialisation()
{
    cli();                           // Désactiver les interruptions globales
    DDRA |= (1 << PA0) | (1 << PA1); // Sorties pour les lumières
    DDRD &= ~(1 << PD2);             // Entrée pour le bouton
    EIMSK |= (1 << INT0);            // Activer INT0
    EIMSK |= (1 << PCINT0);          // Activer PCINT0
    EICRA |= (1 << ISC00);           // Déclenchement INT0 sur front quelconque
    sei();                           // Activer les interruptions globales
}
void demarrerMinuterie(uint16_t duree)
{
    gMinuterieExpiree = 0;
    for (uint16_t i = 0; i < duree; i += 10){
        _delay_ms(10);
    }
    gMinuterieExpiree = 1;
    // Configuration de la minuterie pour une durée donnée
    // (implémentation spécifique dépendante du matériel)
}
ISR(INT0_vect)
{
    gBoutonPoussoir = 1;
    _delay_ms(30); // Délai de rebond
    if ((PIND & (1 << PD2)) != 0)
    {
        gBoutonPoussoir = 0;
        demarrerMinuterie(1000); // Démarrer la minuterie pour 1000 ms
    }
    EIFR |= (1 << INTF0); // Clear INT0 flag
}
void flasherLumiere()
{
    PORTA |= (1 << PA0);  // Allumer la lumière verte
    _delay_ms(10);        // Durée d'allumage
    PORTA &= ~(1 << PA0); // Éteindre la lumière verte
}
ISR(PCINT0_vect)
{
    gMinuterieExpiree = 1;
}
int main()
{
    initialisation();
    while (true)
    {
        _delay_ms(10000);
        flasherLumiere();
        demarrerMinuterie(1000);
        if (gBoutonPoussoir)
        {
            PORTA |= (1 << PA1);  // Allumer la lumière rouge
            _delay_ms(2000);      // Durée d'allumage
            PORTA &= ~(1 << PA1); // Éteindre la lumière rouge
        }
        else
        {
            PORTA |= (1 << PA0);  // Allumer la lumière verte
            _delay_ms(2000);      // Durée d'allumage
            PORTA &= ~(1 << PA0); // Éteindre la lumière verte
        }
    }
}
