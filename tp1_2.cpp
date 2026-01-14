/*
 * Nom: compteur 32 bits
 * Copyright (C) 2005 Matthew Khouzam
 * License http://www.gnu.org/copyleft/gpl.html GNU/GPL

 * Description: Ceci est un exemple simple de programme
 * Version: 1.1

 *
 * Version 1.2 par Jérôme Collin, août 2023

 *
 */

#include <avr/io.h>
#define F_CPU 8000000
#include <util/delay.h>

int main()
{
  // Les 4 ports sont en mode sortie
  DDRA = 0xFF;         // 11111111
  DDRB = 0xFF;         // 11111111
  DDRC = 0xFF;         // 11111111
  DDRD = 0xFF;         // 11111111
  DDRD &= ~(1 << PD2); // 11111011

  // c'est un compteur de 32 bits d'où le type unsigned long
  unsigned long compteur = 0;
  const int vert = 0b01;
  const int delai = 500;
  const int rouge = 0b10;
  const int eteint = 0b00;
  // const int bleu = 0b11;
  // const int x = 0b10100110;
  // const int xx = 0b10101010;
  // const int y = 0b01010101;
  // int temp = xx;
  PORTA = vert;
  while (true)

  {
    if ((PIND & (1 << PD2)) != 0)
    {
      
      _delay_ms(delai);
      if (PORTA == vert)
      {
        PORTA = rouge;
      }
      else
      {
        PORTA = vert;
      }
    }
    else
    {
      PORTA = eteint;
    }
    // Le port D prend les bits de 0 a 7 du compteur
    // autrement dit, l'octet de poids le plus faible.
    // PORTD = compteur;

    // // Le port C prend les bits de 8 à 15 du compteur
    // // en faisant un decallage de 8 bits.
    // PORTC = compteur >> 8;

    // // Le port B prend les bits de 16 à 23 du compteur
    // // en faisant un decallage de 16 bits.
    // PORTB = bleu;

    // // Le port B prend les bits de 24 à 31 du compteur
    // // en faisant un decallage de 24 bits.
    // if (compteur > 0x0FFF)
    // {
    //   PORTA = eteint;
    // }
    // else
    // {
    //   PORTA = temp;
    //   if (temp == xx)
    //   {
    //     temp = y;
    //   }
    //   else
    //   {
    //     temp = xx;
    //   }
    //   if (temp == xx)
    //   {
    //     // attente active
    //     for (volatile int i = 0; i < 5; i++)

    //   }
    // }

    // if (compteur > 0x9FFF)
    // {
    //   compteur = 0;
    // }
    // // si le compteur est à 0xFFFF, il revient à 0 naturellement
    compteur++;
  }

  return 0;
}
