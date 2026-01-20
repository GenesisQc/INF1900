/*
    pb2.cpp - TP2 INF1900
    Auteur: Laurent Corbeil et Tristan Gagne
    Date: Lundi 2 Fevrier 2026
    Description: Programme pour contrôler une lumière à l'aide d'un bouton poussoir avec un état ambre.

    Table de vérité de la machine à états :
    -----------------------------------------------------
    Etat present   & A &  Etat suivant   & Sortie Z \\
    Init           & 0 &  Init           & Rouge    \\
    Init           & 1 &  AppyerAmbre    & Rouge    \\
    AppuyerAmbre   & 0 &  RelacheVert    & Ambre    \\
    AppuyerAmbre   & 1 &  AppuyerAmbre   & Ambre    \\
    RelacherVert   & 0 &  RelacherVert   & Vert     \\
    RelacherVert   & 1 &  AppuyerRouge   & Vert     \\
    AppuyerRouge   & 0 &  RelacherEteint & Rouge    \\
    AppuyerRouge   & 1 &  AppuyerRouge   & Rouge    \\
    RelacherEteint & 0 &  RelacherEteint & Eteint   \\
    RelacherEteint & 1 &  AppuyerVert    & Eteint   \\
    AppuyerVert    & 0 &  Init           & Vert     \\
    AppuyerVert    & 1 &  AppuyerVert    & Vert     \\
    -----------------------------------------------------
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
const int debounceDelay = 50;      // Délai de rebond en millisecondes
const int DelaiAffichageAmbre = 5; // Délai d'affichage pour la couleur ambre en millisecondes
const int Vert = 0b00000001;
const int Rouge = 0b00000010;
const int Eteint = 0b00000000;

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
        _delay_ms(debounceDelay); // Attendre pour le rebond
        if (PIND & (1 << PD2))
        {
            return true; // Le bouton est appuyé
        }
    }
    else
    {
        return false; // Le bouton n'est pas appuyé
    }
    // Vérifier si le bouton poussoir est appuyé (niveau bas)
    return (PIND & (1 << PD2));
}
void AllumerLumiere(int couleur)
{
    PORTA = couleur; // Allumer la lumière avec la couleur spécifiée
}
void AfficherAmbre()
{
    // Afficher la couleur ambre en alternant rouge et vert rapidement
    
    AllumerLumiere(Rouge);
    _delay_ms(DelaiAffichageAmbre);
    AllumerLumiere(Vert);
    _delay_ms(DelaiAffichageAmbre);
}
int main()
{
    // Les 4 ports sont en mode sortie
    DDRA = 0xFF;         // 11111111
    DDRB = 0xFF;         // 11111111
    DDRC = 0xFF;         // 11111111
    DDRD &= ~(1 << PD2); // 11111011

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