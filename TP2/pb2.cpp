/*
    pb2.cpp - TP2 INF1900
    Auteur: Laurent Corbeil et Tristan Gagne
    Date: Lundi 2 Fevrier 2026
    Description: Programme pour contrôler une lumière à l'aide d'un bouton poussoir avec un état ambre.

    Table de vérité de la machine à états :
    -------------------------------------------------------------
    Etat present   & Button &  Etat suivant   & Sortie Couleur \\
    Init           &    0   &  Init           & Rouge          \\
    Init           &    1   &  AppyerAmbre    & Rouge          \\
    AppuyerAmbre   &    0   &  RelacheVert    & Ambre          \\
    AppuyerAmbre   &    1   &  AppuyerAmbre   & Ambre          \\
    RelacherVert   &    0   &  RelacherVert   & Vert           \\
    RelacherVert   &    1   &  AppuyerRouge   & Vert           \\
    AppuyerRouge   &    0   &  RelacherEteint & Rouge          \\
    AppuyerRouge   &    1   &  AppuyerRouge   & Rouge          \\
    RelacherEteint &    0   &  RelacherEteint & Eteint         \\
    RelacherEteint &    1   &  AppuyerVert    & Eteint         \\
    AppuyerVert    &    0   &  Init           & Vert           \\
    AppuyerVert    &    1   &  AppuyerVert    & Vert           \\
    -------------------------------------------------------------
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
#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
const uint8_t debounceDelay = 50;      // ms
const uint8_t DelaiAffichageAmbre = 5; // ms
const uint8_t Rouge = (1 << PA0);
const uint8_t Vert = (1 << PA1);
const uint8_t Eteint = 0x00;

enum Etat
{
    Init,
    AppuyerAmbre,
    RelacherVert,
    AppuyerRouge,
    RelacherEteint,
    AppuyerVert
};
bool BoutonPoussoirAppuyer()
{
    if (PIND & (1 << PD2))
    {
        _delay_ms(debounceDelay);
        return (PIND & (1 << PD2));
    }
    return false;
}
void AllumerLumiere(uint8_t couleur)
{
    switch (couleur)
    {
    case Rouge:
        PORTA |= (1 << PA0);
        PORTA &= ~(1 << PA1);

        break;
    case Vert:
        PORTA |= (1 << PA1);
        PORTA &= ~(1 << PA0);
        break;
    case Eteint:
        PORTA &= ~(1 << PA0 | 1 << PA1);
        break;

    default:
        break;
    }
}
void AfficherAmbre()
{
    AllumerLumiere(Rouge);
    _delay_ms(DelaiAffichageAmbre);
    AllumerLumiere(Vert);
    _delay_ms(DelaiAffichageAmbre);
}
int main()
{
    DDRA |= (1 << PA0) | (1 << PA1);
    DDRD &= ~(1 << PD2);

    Etat etat = Init;
    while (true)
    {
        switch (etat)
        {
        case Init:
            if (BoutonPoussoirAppuyer())
            {
                etat = AppuyerAmbre;
            }
            AllumerLumiere(Rouge);
            break;
        case AppuyerAmbre:
            if (!BoutonPoussoirAppuyer())
            {
                etat = RelacherVert;
            }
            AfficherAmbre();
            break;
        case RelacherVert:
            AllumerLumiere(Vert);
            if (BoutonPoussoirAppuyer())
            {
                etat = AppuyerRouge;
            }
            break;
        case AppuyerRouge:
            if (!BoutonPoussoirAppuyer())
            {
                etat = RelacherEteint;
            }
            AllumerLumiere(Rouge);
            break;
        case RelacherEteint:
            AllumerLumiere(Eteint);
            if (BoutonPoussoirAppuyer())
            {
                etat = AppuyerVert;
            }
            break;
        case AppuyerVert:
            if (!BoutonPoussoirAppuyer())
            {
                etat = Init;
            }
            AllumerLumiere(Vert);
            break;
        }
    }

    return 0;
}