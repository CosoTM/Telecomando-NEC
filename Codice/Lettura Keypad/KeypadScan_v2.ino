#include <LiquidCrystal_I2C.h>

#define I2C_ADDR    0x27
#define LCD_COLUMNS 16
#define LCD_LINES   2

LiquidCrystal_I2C lcd(I2C_ADDR, LCD_COLUMNS, LCD_LINES);

// !!! LO SCHERMO LCD E' PRESENTE SOLO PER MOSTRARE IL FUNZIONAMENTO DI QUESTO CODICE. 
// NON SARA' PRESENTE IN NESSUNA ITERAZIONE DEL PRODOTTO FINALE. !!!

#define NO_KEY 20
volatile uint8_t pressed_key = NO_KEY;

void setup() {
  lcd.init();
  lcd.backlight();

  DDRD = 0x78; 
  PORTD = 0x7F; 
}

// !!! FUNZIONE LOOP UTILIZZATA SOLAMENTE PER MOSTRARE IL FUNZIONAMENTO DI QUESTO CODICE !!!
void loop() {
  pressed_key = keypadScan();
  lcd.setCursor(0,0);
  lcd.print("Premuto: ");
  lcd.setCursor(9,0);
  if(pressed_key == NO_KEY){
    lcd.print("  ");
  }else{
    lcd.print(pressed_key);
  }
}

// Funzione keypadRead()
uint8_t keypadScan(){
  uint8_t keyCode = NO_KEY; 
                            
  for (uint8_t i = 0; i < 4 ; ++i){
    // Operazione Bitwise al posto dell array.
    PORTD = ~_BV(6-i); 
    for (uint8_t j = 0 ; j < 3 ; ++j){
      if ((PIND & (_BV(j))) == 0){
         keyCode = i * 3 + j; 
      }
    }
  }
  
  // Ho notato che, durante e operazioni della funzione, la "direzione" dei pin non viene cambiata
  // ed è di conseguenza inutile resettarle a fine funzione. Si può quindi omettere:
  // DDRD = 0x78;

  PORTD = 0x7F;
  
  return keyCode;
}

