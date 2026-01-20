/*
    TP 2 - Problème 1
    Nom: Laurent Corbeil et Tristan Gagne
    Date: Lundi 2 Fevrier 2026
    Description: Machine a etats pour une lumiere activer par un bouton poussoir


*/
/*
 * ========== I/O IDENTIFICATION (CONNEXIONS SUR LE ROBOT) =============
 *
 *
 * ┌───────────────────────────────────────────────────────────────────┐
 * │                    ATmega324PA - Pinout                           │
 * ├───────────────────────────────────────────────────────────────────┤
 * │                                                                   │
 * │   PORTA            PORTB           PORTC          PORTD           │
 * │  ┌───────┐        ┌──────┐         ┌──────┐       ┌──────┐        │
 * │  │ PA0 ●─│→B Lum  │PB0 ○ │         │PC0 ○ │       │PD0 ○ │        │
 * │  │ PA1 ●─│→B Lum  │PB1 ○ │         │PC1 ○ │       │PD1 ○ │        │
 * │  │ PA2 ○ │        │PB2 ○ │         │PC2 ○ │       │PD2 ●─│→B-INT0 │
 * │  │ PA3 ○ │        │PB3 ○ │         │PC3 ○ │       │PD3 ●─│→B-INT0 │
 * │  │ PA4 ○ │        │PB4 ○ │         │PC4 ○ │       │PD4 ○ │        │
 * │  │ PA5 ○ │        │PB5 ○ │         │PC5 ○ │       │PD5 ○ │        │
 * │  │ PA6 ○ │        │PB6 ○ │         │PC6 ○ │       │PD6 ○ │        │
 * │  │ PA7 ○ │        │PB7 ○ │         │PC7 ○ │       │PD7 ○ │        │
 * │  └───────┘        └──────┘         └──────┘       └──────┘        │
 * │                                                                   │
 * │  ● = Utilisé          ○ = Non utilisé                             │
 * └───────────────────────────────────────────────────────────────────┘
 * LEGENDE :
 * B Lum  : lumière
 * B-INT0 : bouton
 * =====================================================================
 */

#include <avr/io.h>
#define F_CPU 8000000
#include <util/delay.h>
const int debounceDelay = 50;   // Délai de rebond en millisecondes
const int DureeAllumage = 2000; // Durée d'allumage de la lumière en millisecondes
enum Etat
{
    Init,
    AttendreRelache,
    AllumerVert
};
void AllumerLumiere(int couleur, enum Etat &etat)
{
    etat = (Etat)AllumerVert;
    PORTA = couleur;          // Allumer la lumière avec la couleur spécifiée
    _delay_ms(DureeAllumage); // Attendre la durée d'allumage
    PORTA = 0x00;             // Éteindre la lumière
    etat = (Etat)Init;
}
bool BoutonPoussoirAppuyer()
{
    if (PIND & (1 << PD2))
    {
        _delay_ms(debounceDelay); // Attendre pour le rebond
        if (PIND & (1 << PD2))
        {
            return true; // Le bouton est appuyé
        }
    }
    return false; // Le bouton n'est pas appuyé
}
int main()
{
    // Les 4 ports sont en mode sortie sauf pin PD2
    DDRA = 0xFF;         // 11111111
    DDRB = 0xFF;         // 11111111
    DDRC = 0xFF;         // 11111111
    DDRD &= ~(1 << PD2); // 11111011

    Etat etat = Init;
    const int vert = 0b00000001;
    int compteur = 0;
    bool premierAppui = true;
    while (true)
    {
        if (BoutonPoussoirAppuyer() && etat == (Etat)Init)
        {
            if (premierAppui)
            {
                etat = (Etat)AttendreRelache;
                premierAppui = false;
            }
        }
        else
        {
            if (!BoutonPoussoirAppuyer() && etat == (Etat)AttendreRelache)
            {
                compteur++;
                etat = (Etat)Init;
            }
            premierAppui = true;
        }

        if (compteur == 3)
        {
            AllumerLumiere(vert, etat);
            compteur = 0;
        }
    }
    return 0;
}