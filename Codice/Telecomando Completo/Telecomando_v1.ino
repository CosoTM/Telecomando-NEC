// Quanti ms di uno stesso segnale per contarlo privo di rimbalzi
#define DEBOUNCE_TIME 5 
// Indica lo stato in cui nessun tasto del keypad è cliccato. In questo caso può essere qualunque valore >12
#define NO_KEY 20 
// L'Indirizzo del dispositivo. Staccato dai Comandi, perche l'Indirizzo è lo stesso per tutti i comandi.
#define REMOTE_ADDRESS 0x00

//Codici comandi NEC
const uint8_t NEC_Codes[] = {
                             0x30, //1 
                             0x18, //2
                             0x7A, //3
                             0x10, //4
                             0x38, //5
                             0x5A, //6
                             0x42, //7
                             0x4A, //8
                             0x52, //9
                             0xA8, //Volume+
                             0x68, //0
                             0xE0  //Volume-
                            };

volatile uint8_t pressedKey = NO_KEY;
volatile uint8_t savedKey = 0;
volatile uint8_t keyCounted = 0;
volatile uint8_t key;

volatile uint8_t counter2 = 0;
uint8_t counter1 = 0;

uint8_t phaseCounter = 0;
uint8_t bitCount = 0;
uint8_t burstCount = 0;
uint8_t stateCount[2];

// Inizializzazione
void setup() {
  DDRB |= 0x08;  // Pin 11 in Output per la trasmissione messaggio NEC

  // Pin da 0 a 2 a INPUT_PULLUP e Pin da 3 a 6 ad OUTPUT
  DDRD |= 0x78;
  PORTD |= 0x7F;

  // Timer0 utilizzato per eseguire Debouncing e far funzionare Keypad
  TCCR0A = _BV(WGM01);                   // Modalità CTC
  TCCR0B = _BV(CS01) | _BV(CS00);        // Prescaler 64
  OCR0A = 249;                           // Top del Timer a 249; Valore in Tempo:
                                         // = (249 * 64 * 62.5ns) = 1ms
  TIMSK0 = _BV(OCIE0A);                  // Attivo "Compare Match A Interrupt"

  // Timer1 per controllare l'accensione della Carrier Frequency
  TCCR1A = 0;           
  TCCR1B = _BV(WGM12);  // Timer in Modalità CTC con TOP contenuto in OCR1A
  TIMSK1 = _BV(OCIE1A); // Attivazione di Interrupt Output Compare A Match
  OCR1A  = 8999;        // Top del timer a 8999

  // Timer2 per generare la Carrier Frequency di 38KHz
  TCCR2A = _BV(WGM21);  // Timer in Modalità CTC
  TCCR2B = 0;
  OCR2A =  209;
}

// Routine Anti-Rimbalzo
ISR(TIMER0_COMPA_vect) {
  pressedKey = keypadScan();
  
  if (savedKey != pressedKey || keyCounted > DEBOUNCE_TIME){
    savedKey = pressedKey;
    keyCounted = 0;
  } 

  if (keyCounted++ == DEBOUNCE_TIME && phaseCounter == 0 && pressedKey != NO_KEY)
  {
    key = NEC_Codes[pressedKey];
    phaseChange();
  }
}

//Routine di Conteggio
ISR (TIMER1_COMPA_vect) {
  if (++counter2 == counter1) {
    burstGenerator();

    TCCR2A ^= _BV (COM2A0);
    counter2 = 0;
  }
}

//Funzione Conta Fasi
void phaseChange() {
  switch (phaseCounter) {
    case 0: { // Inizio
      stateCount[0] = 16;
      stateCount[1] = 8;
      burstGenerator();

      phaseCounter++;

      TCCR1B |= _BV (CS10);
      carrier_toggle();
    } break;

    case 1: { // Address
      buildNEC(REMOTE_ADDRESS);
    } break;

    case 2: { // ~Address
      buildNEC(~REMOTE_ADDRESS);
    } break;

    case 3: { // Codice
      buildNEC(key);
    } break;

    case 4: { // ~Codice
      buildNEC(~key);
    } break;

    case 5: { // Impulso finale
      counter1 = 1;
      phaseCounter++;
    } break;

    case 6: { // Fine
      carrier_toggle();
      TCCR1B &= ~_BV (CS10);

      counter1 = 0;
      phaseCounter = 0;
      burstCount = 0;
      bitCount = 0;
    } break;
  }
}

void buildNEC(uint8_t code) {
  if (bitCount < 8) {
    burstCount = 0;
    stateCount[0] = 1;
    stateCount[1] = ((code >> bitCount++) & 1) ? 3 : 1;
  } else {
    phaseCounter++;
    bitCount = 0;
  }

  burstGenerator();
}

void burstGenerator() {
  if (burstCount < 2)  counter1 = stateCount[burstCount++];
  else phaseChange();
}

//Funzione di scan del Keypad
uint8_t keypadScan() {
  uint8_t keyCode = NO_KEY;
  for (uint8_t i = 0; i < 4 ; ++i) {
    PORTD = ~_BV(6-i);
    for (uint8_t j = 0 ; j < 3 ; ++j) {
      if (!(PIND & _BV(j))) {
        keyCode = i * 3 + j;
        break;
      }
    }
  }
  PORTD = 0x7F;
  return keyCode;
}

void carrier_toggle(){
  TCCR2B ^= _BV (CS20);  // Prescaler sul Timer2 di 1 (No prescaler in praticà)
                         // Top del Timer a 209 -> valore in tempo = (OCR2A+1 * prescaler * clockSpeed) =
                         // = (210 * 1 * 62.5ns)  = 13.125us, quindi 1/(2 * 13.125us) = 38095 Hz -> 38KHz
  TCCR2A ^= _BV (COM2A0);
}

void loop() {
  // vuoto
}
