void setup() {
  DDRB |= 0x08; // Direzione Pin 11 in OUTPUT

  TCCR2A = _BV(WGM21) | _BV (COM2A0);  // Timer in Modalità CTC e attivo "Alterna OC2A ad ogni Compare Match"
  TCCR2B = _BV(CS20);   // Prescaler di 1
  //TIMSK2 = _BV(OCIE2A); Non ci serve più attivare "Interrupt on Compare Match" per alternare lo stato del pin
  OCR2A = 209;         
}


void loop() {}
