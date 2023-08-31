#include <LiquidCrystal_I2C.h>

#define I2C_ADDR    0x27
#define LCD_COLUMNS 16
#define LCD_LINES   2

LiquidCrystal_I2C lcd(I2C_ADDR, LCD_COLUMNS, LCD_LINES);

// !!! LO SCHERMO LCD E' PRESENTE SOLO PER MOSTRARE IL FUNZIONAMENTO DI QUESTO CODICE. 
// NON SARA' PRESENTE IN NESSUNA ITERAZIONE DEL PRODOTTO FINALE. !!!

#define NO_KEY 20
#define DEBOUNCE_COUNT 5 // Rappresenta la costante "x" nel flowchart

volatile uint8_t pressedKey = NO_KEY;
volatile uint8_t savedKey = 0;
volatile uint8_t keyCounted = 0;

void setup() {
  lcd.init();
  lcd.backlight();

  // Pin 11 in Output per il LED
  DDRB |= 0x08; 

  // Imposto i pin per il keypad
  DDRD = 0x78;
  PORTD = 0x7F;

  //Timer0 utilizzato per eseguire Debouncing e far funzionare Keypad
  TCCR0A = _BV(WGM01);                   // Modalità CTC
  TCCR0B = _BV(CS01) | _BV(CS00) ;       // Prescaler 64
  OCR0A = 249;                           // Top del Timer a 249; Valore in Tempo:
                                         // = (249 * 64 * 62.5ns) = 1ms
  TIMSK0 = _BV(OCIE0A);                  // Attivo "Compare Match A Interrupt"
}

// !!! FUNZIONE LOOP UTILIZZATA SOLAMENTE PER MOSTRARE IL FUNZIONAMENTO DI QUESTO CODICE !!!
void loop() {
  lcd.setCursor(0,0);
  lcd.print("Pressed: ");
  lcd.setCursor(9,0);
  if(pressedKey == NO_KEY){
    lcd.print("  ");
  }else{
    lcd.print(pressedKey+1);
  }
}

// Funzione di KeypadScan
uint8_t keypadScan(){
  uint8_t keyCode = NO_KEY; 
                            
  for (uint8_t i = 0; i < 4 ; ++i){
    PORTD = ~_BV(6-i); 
    for (uint8_t j = 0 ; j < 3 ; ++j){
      if ((PIND & (_BV(j))) == 0){
         keyCode = i * 3 + j; 
      }
    }
  }
  PORTD = 0x7F;
  return keyCode;
}

// La routine di Interrupt verrà eseguita ogni millisecondo. 
ISR(TIMER0_COMPA_vect){
    // Esegue keypadScan che ci restituisce il numero del tasto premuto.
    pressedKey = keypadScan();
   
    // Se il tasto che il keypadScan ci restituisce non è uguale a quello di savedKey, il keyCounted viene
    // azzerato, e viene salvato il nuovo tasto in savedKey.
    if(savedKey != pressedKey){
      savedKey = pressedKey;
      keyCounted = 0;
    }

    // Aumenta ogni volta keyCounted di 1, e se questo raggiunge DEBOUNCE_COUNT, il rimbalzo e considerato
    // rimosso, e possiamo quindi eseguire la nostra funzione con sicurezza.
    if(keyCounted++ == DEBOUNCE_COUNT ){
      funzione();
      keyCounted = 0;
    } 
}

// Semplice funzione per mostrare il funzionamento del codice.
void funzione(){
  if(pressedKey != NO_KEY){
    PORTB |= _BV(3);
  }else{
    PORTB &= ~_BV(3);
  }
}
