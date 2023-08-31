void setup() {
  DDRB |= 0x08; // Direzione Pin 11 in OUTPUT

  // Timer2 per generare la Carrier Frequency di 38KHz
  TCCR2A = _BV(WGM21);   // Timer in Modalità CTC
  TCCR2B = _BV(CS20);    // Prescaler sul di 1
  TIMSK2 = _BV(OCIE2A);  // Attivazione di Interrupt Output Compare A Match
  OCR2A = 209;           // Top del Timer a 209 -> valore in tempo = (OCR2A+1 * prescaler * clockSpeed) =
                         // = (210 * 1 * 62.5ns)  = 13.125us (76.19 kHz), quindi (2 * 13.125us) = 26.5us (38.095 kHz)
}

ISR(TIMER2_COMPA_vect) {
    PORTB ^= _BV(3); // Alterno lo stato del pin ad ogni interrupt
}

void loop() {}
