/*
    TP 2 - Problème 1
    Nom: Laurent Corbeil et Tristan Gagne
    Date: Lundi 2 Fevrier 2026
    Description: Machine a etats pour une lumiere activer par un bouton poussoir


*/
/*
    Table de vérité de la machine à états :
    -------------------------------------------------------------
    Etat present   & Button &  Etat suivant   & Sortie Couleur  \\
    Init           &  0   &  Init             & Eteint          \\
    Init           &  1   &  Appuyer1         & Eteint          \\
    Appuyer1       &  0   &  Relacher1        & Eteint          \\
    Appuyer1       &  1   &  Appuyer1         & Eteint          \\
    Relacher1      &  0   &  Relacher1        & Eteint          \\
    Relacher1      &  1   &  Appuyer2         & Eteint          \\
    Appuyer2       &  0   &  Relacher2        & Eteint          \\
    Appuyer2       &  1   &  Appuyer2         & Eteint          \\
    Relacher2      &  0   &  Relacher2        & Eteint          \\
    Relacher2      &  1   &  Appuyer3         & Eteint          \\
    Appuyer3       &  0   &  AllumerVert      & Eteint          \\
    Appuyer3       &  1   &  Appuyer3         & Eteint          \\
    AllumerVert    &  X   &  Init             & Vert            \\
    AllumerVert    &  X   &  Init             & Vert            \\
    -------------------------------------------------------------
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
#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
enum class Etat;
const uint8_t DelaiRebond = 30;      // Délai ms
const uint16_t DureeAllumage = 2000; // Durée ms
void AugmenterEtat(volatile Etat &etat);
void GererEtat(volatile Etat &etat);
enum class Etat
{
    Init,
    Appuyer1,
    Relacher1,
    Appuyer2,
    Relacher2,
    Appuyer3,
    AllumerVert
};
volatile Etat etat = Etat::Init;
void AllumerLumiere(volatile Etat &etat)
{
    etat = Etat::AllumerVert;
    PORTA |= (1 << PA0);
    PORTA &= ~(1 << PA1);
    _delay_ms(DureeAllumage);
    PORTA &= ~((1 << PA0) | (1 << PA1));
    etat = Etat::Init;
}
ISR(INT0_vect)
{
    _delay_ms(DelaiRebond);
    if (PIND & (1 << PD2))
    {
        AugmenterEtat(etat);
        EICRA &= ~((0 << ISC01) | (0 << ISC00)); // Front montant
    }
    EIFR |= (1 << INTF0); // Clear INT0 flag
}
void initialisation ()
{
    cli();                             // Désactiver les interruptions globales
    DDRA |= (1 << PA0) | (1 << PA1); // Sorties pour les lumières
    DDRD &= ~(1 << PD2);              // Entrée pour le bouton
    EIMSK |= (1 << INT0);             // Activer INT0
    EICRA |= (1 << ISC01) | (1 << ISC00); // Front montant sur INT0
    sei();                             // Activer les interruptions globales
}
void AugmenterEtat(volatile Etat &etat)
{
    switch (etat)
    {
    case Etat::Init:
        etat = Etat::Appuyer1;
        break;
    case Etat::Appuyer1:
        etat = Etat::Relacher1;
        break;
    case Etat::Relacher1:
        etat = Etat::Appuyer2;
        break;
    case Etat::Appuyer2:
        etat = Etat::Relacher2;
        break;
    case Etat::Relacher2:
        etat = Etat::Appuyer3;
        break;
    case Etat::Appuyer3:
        etat = Etat::AllumerVert;
        break;
    default:
        break;
    }
}
void GererEtat(volatile Etat &etat)
{
    switch (etat)
    {
    case Etat::AllumerVert:
        AllumerLumiere(etat);
        break;
    default:
        break;
    }
}

int main()
{
    initialisation();
    while (true)
    {
        GererEtat(etat);
    }
    return 0;
}