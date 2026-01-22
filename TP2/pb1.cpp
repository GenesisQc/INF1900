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
const uint8_t DelaiRebond = 50;      // Délai ms
const uint16_t DureeAllumage = 2000; // Durée ms
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
void AllumerLumiere(Etat &etat)
{
    etat = Etat::AllumerVert;
    PORTA |= (1 << PA0);
    PORTA &= ~(1 << PA1);
    _delay_ms(DureeAllumage);
    PORTA &= ~((1 << PA0) | (1 << PA1));
    etat = Etat::Init;
}
bool BoutonPoussoirAppuyer()
{
    if (PIND & (1 << PD2))
    {
        _delay_ms(DelaiRebond);
        return (PIND & (1 << PD2));
    }
    return false; 
}
void GererEtat(Etat &etat)
{
    switch (etat)
    {
    case Etat::Init:
        if (BoutonPoussoirAppuyer())
        {
            etat = Etat::Appuyer1;
        }
        break;
    case Etat::Appuyer1:
        if (!BoutonPoussoirAppuyer())
        {
            etat = Etat::Relacher1;
        }
        break;
    case Etat::Relacher1:
        if (BoutonPoussoirAppuyer())
        {
            etat = Etat::Appuyer2;
        }
        break;
    case Etat::Appuyer2:
        if (!BoutonPoussoirAppuyer())
        {
            etat = Etat::Relacher2;
        }
        break;
    case Etat::Relacher2:
        if (BoutonPoussoirAppuyer())
        {
            etat = Etat::Appuyer3;
        }
        break;
    case Etat::Appuyer3:
        if (!BoutonPoussoirAppuyer())
        {
            etat = Etat::AllumerVert;
        }
        break;
    case Etat::AllumerVert:
        AllumerLumiere(etat);
        break;
    }
}

int main()
{
    DDRA |= (1 << PA0) | (1 << PA1);
    DDRD &= ~(1 << PD2);

    Etat etat = Etat::Init;

    while (true)
    {
        GererEtat(etat);
    }
    return 0;
}