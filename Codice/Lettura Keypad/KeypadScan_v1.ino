#include <LiquidCrystal_I2C.h>

#define I2C_ADDR    0x27
#define LCD_COLUMNS 16
#define LCD_LINES   2

LiquidCrystal_I2C lcd(I2C_ADDR, LCD_COLUMNS, LCD_LINES);

// !!! LO SCHERMO LCD E' PRESENTE SOLO PER MOSTRARE IL FUNZIONAMENTO DI QUESTO CODICE. 
// NON SARA' PRESENTE IN NESSUNA ITERAZIONE DEL PRODOTTO FINALE. !!!

#define NO_KEY 20
volatile uint8_t pressed_key = NO_KEY;

// Nella funzione setup, setto le righe ad OUTPUT HIGH e le colonne ad INPUT PULL-UP
// I pin delle righe sono da 6 a 3 mentre i pin delle colonne sono da 2 a 0
void setup() {
  lcd.init();
  lcd.backlight();

  // DDRD: configura la direzione dei pin (0 = INPUT / 1 = OUTPUT)
  DDRD = 0x78; // = 0111 1000 quindi i pin da 6 a 3 configurati come OUTPUT, pin da 2 a 0 come INPUT

  // PORTD: configura lo stato dei pin (0 = LOW / 1 = HIGH; OUTPUT ed 1 = INPUT PULL-UP )
  PORTD = 0x7F; // = 0111 1111 quindi i pin da 6 a 3 sono configurati ad HIGH, 
                // i pin da 2 a 0, essendo configurati INPUT, diventano INPUT PULL-UP
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
  // In shiftArray sono contenuti i valori delle righe con una riga alla volta a LOW:
  // 0xBF: 1011 1111
  // 0xDF: 1101 1111
  // 0xEF: 1110 1111
  // 0xF7: 1111 0111
  uint8_t shiftArray[4]={0xBF, 0xDF, 0xEF, 0xF7};
  uint8_t keyCode = NO_KEY; // Inizzializzato a NO_KEY. Se nessun tasto è premuto, 
                            // questa funzione restituisce semplicemente NO_KEY.

  for (uint8_t i = 0; i < 4 ; ++i){
    // Nel ciclo, PORTD viene cambiato negli elementi di shiftArray, cambiando quindi lo stato dei pin
    // delle righe da HIGH a LOW.
    PORTD = shiftArray[i];
    for (uint8_t j = 0 ; j < 3 ; ++j){
      // All'interno del secondo ciclo, controlliamo se al cambio della riga da HIGH a LOW, c'è anche
      // un cambio di una colonna da HIGH a LOW. Il controllo viene fatto facendo un operazione 
      // bitwise AND fra gli stati dei pin (PIND) e 1 shiftato a sinistra j volte. 
      if ((PIND & (_BV(j))) == 0){
         keyCode = i * 3 + j; 
      }
    }
  }
  
  // Alla fine della funzione, i pin vengono ripristinati allo stato originariamente settato in setup().
  DDRD = 0x78;
  PORTD = 0x7F;
  
  return keyCode;
}

