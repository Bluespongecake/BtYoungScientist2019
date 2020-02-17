//this is the code that will write the input and binary transmission to the LCD

// this is the library for the controller board "I2C"
#include <Wire.h> 

// pretty self explanatory
#include <LiquidCrystal_I2C.h>

//this is the I2C adress, returned by the I2C finder script that is on max's PC if needed
const int I2C = 0x27; 
//yeah its in hexadecimal. In decimal this is 39. Why? Ask sunfounder

#define Clock 3
#define Data 4

//---------------------------------------------------------------------------


const char *message = "Hello all"; // message to be sent


//---------------------------------------------------------------------------

//defining the pins for the LCD (from the I2C (a type of bus))
LiquidCrystal_I2C lcd(I2C,16,2); //16 char 2 rows

void setup(){
  //define output pins
  pinMode(TX_CLOCK, OUTPUT);
  pinMode(TX_DATA, OUTPUT);

  //initiate LCD
  lcd.init();
  lcd.backlight();
  
  Serial.begin(9600);                                              //always useful

  lcd.print("starting tx...");
  delay(500);
  lcd.clear();
  lcd.print(message);

  //print progress
  lcd.setCursor(2, 2);
  lcd.print(strlen(message));
  lcd.setCursor(0, 2);
  lcd.print("0/");

  //main operation. bytec = byte counter
  for (int byte_idx = 0; bytec < strlen(message); bytec++) {
    char tbyte = message[bytec];

    // Clear the 3rd line of the display
    lcd.noCursor();
    lcd.setCursor(0, 2);
    lcd.print("        ");
    lcd.setCursor(byte_idx, 0);
    lcd.cursor();

    for (int bitc = 0; bitc < 8; bitc++) {                         //bitc is bit counter
      bool tbit = tbyte & (0x80 >> bitc);                   //not sure what this is

      digitalWrite(DATA, tbit);
      delay((1000 / TX_RATE) / 2);

      // Update the LCD and serial monitor
      lcd.noCursor();
      lcd.setCursor(bitc, 1);
      lcd.print(tbit ? "1" : "0");
      serial.println(tbit ? "1" : "0");
      lcd.setCursor(byte_idx, 0);
      lcd.cursor();

      // Pulse clock
      digitalWrite(TX_CLOCK, HIGH);
      delay((1000 / TX_RATE) / 2);
      digitalWrite(TX_CLOCK, LOW);      
    }
  }
  digitalWrite(TX_DATA, LOW);
}
    


  
}

void loop(){
  
}
