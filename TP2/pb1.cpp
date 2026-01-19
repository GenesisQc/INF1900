
/*
    TP 2 - Problème 1
    Nom: Laurent Corbeil et Tristan Gagne
    Date: Lundi 2 Fevrier 2026
    Description: Machine a etats pour une lumiere activer par un bouton poussoir
    
*/

#include <avr/io.h>
#define F_CPU 8000000
#include <util/delay.h>
const int debounceDelay = 50; // Délai de rebond en millisecondes
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
    PORTA = couleur; // Allumer la lumière avec la couleur spécifiée
    _delay_ms(DureeAllumage); // Attendre la durée d'allumage
    PORTA = 0x00; // Éteindre la lumière
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
    else
    {
        return false; // Le bouton n'est pas appuyé
    }
    // Vérifier si le bouton poussoir est appuyé (niveau bas)
    return (PIND & (1 << PD2));
}
int main()
{
    // Les 4 ports sont en mode sortie
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