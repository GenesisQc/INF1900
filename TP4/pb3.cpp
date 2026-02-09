#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


    /*ok laurent du futur ocr1a et ocr1b sont des registres de 8 bits, donc on peut leur assigner directement une valeur entre 0 et 255
     ocr veut oc (output compare register) sa veut dire que si tu met le timer en mode comparaison quand il atteint ce chiffre il va faire quelle
     que chose ensuite de cela 

     ******** TCCR1B TCCRnB – Timer/Counter n Control Register est fait comme cela ICNCn ICESn – WGMn3 WGMn2 CSn2 CSn1 CSn0 *******
     ******** TCCR1A TCCRnA – Timer/Counter n Control Register est fait comme cela COMnA1 COMnA0 COMnB1 COMnB0 – WGMn1 WGMn0 ********

     dans ce registe tu peut setter le prescaler (CSn2 CSn1 CSn0). le prescaler sert a dire de combien tu ralenti la monter du timer par rapport a la clock
     ensuite les WGMn sont les modes de fonctionnement du timer. finalement il y a les COMnA1 COMnA0 COMnB1 COMnB0 qui sont les options de comparaison pour les deux canaux A et B du timer.


    Description: WGMn3 WGMn2 WGMn1 WGMn0

    0 0000Normal0xFFFFImmediateMAX
    1 0001PWM, Phase Correct, 8-bit 0x00FF TOP BOTTOM
    2 0010PWM, Phase Correct, 9-bit 0x01FF TOP BOTTOM
    3 0011PWM, Phase Correct, 10-bit 0x03FF TOP BOTTOM
    4 0100CTCOCRnAImmediateMAX
    5 0101Fast PWM, 8-bit 0x00FF BOTTOM TOP
    6 0110Fast PWM, 9-bit 0x01FF BOTTOM TOP
    7 0111Fast PWM, 10-bit 0x03FF BOTTOM TOP
    8 1000PWM, Phase and Frequency
    Correct ICRn BOTTOM BOTTOM
    9 1001PWM, Phase and Frequency
    Correct OCRnA BOTTOM BOTTOM
    10 1010PWM, Phase Correct ICRn TOP BOTTOM
    11 1011PWM, Phase Correct OCRnA TOP BOTTOM
    12 1100CTCICRnImmediateMAX
    13 1101(Reserved)–––
    14 1110Fast PWM ICRn BOTTOM TOP
    15 1111Fast PWM OCRnA BOTTOM TOP

     
    Description : CSn2 CSn1 CSn0
    000 No clock source (Timer/Counter stopped).
    001 clkI/O/1 (No prescaling) 
    010 clkI/O/8 (From prescaler) ici tu ralenti la montée du timer par 8, donc si ta clock est a 8MHz le timer va monter a 1MHz
    011 clkI/O/64 (From prescaler) ici tu ralenti la montée du timer par 64, donc si ta clock est a 8MHz le timer va monter a 125kHz
    100 clkI/O/256 (From prescaler) ici tu ralenti la montée du timer par 256, donc si ta clock est a 8MHz le timer va monter a 31.25kHz
    101 clkI/O/1024 (From prescaler) ici tu ralenti la montée du timer par 1024, donc si ta clock est a 8MHz le timer va monter a 7.8125kHz
    110 External clock source on Tn pin. Clock on falling edge.
    111 External clock source on Tn pin. Clock on rising edge.
     et le mode de fonctionnement du timer (WGMn3 WGMn2) et les options de comparaison (COMnA1 COMnA0 COMnB1 COMnB0) qui sont dans



TCCRnB*/

void ajustementPwm(uint8_t rapportA, uint8_t rapportB)
{

    // valeur duty cycle
    OCR1A = rapportA;
    OCR1B = rapportB;

    // PWM phase correct 8 bits
    // Clear OC1A/OC1B on compare match (non inversé)
    TCCR1A = (1 << COM1A1) | (1 << COM1B1) | (1 << WGM10);

    // prescaler 8
    TCCR1B = (1 << CS11);

    TCCR1C = 0;
}

int main()
{
    // PD4 et PD5 en sortie (OC1B et OC1A)
    DDRD |= (1 << PD4) | (1 << PD5);

    while (1)
    {
        // 0%
        ajustementPwm(0, 0);
        _delay_ms(2000);

        // 25%
        ajustementPwm(64, 64);
        _delay_ms(2000);

        // 50%
        ajustementPwm(128, 128);
        _delay_ms(2000);

        // 75%
        ajustementPwm(192, 192);
        _delay_ms(2000);

        // 100%
        ajustementPwm(255, 255);
        _delay_ms(2000);
    }
}
