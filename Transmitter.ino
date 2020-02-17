/*-------------------------------------------------------------------------------------------------------------------

            Composed by max mccormack.
            I've never done much coding before so this took a long time to build up to and write,  including many
            tests and failed attempts. This is the version that seems to work


-------------------------------------------------------------------------------------------------------------------*/



#include <LiquidCrystal_I2C.h>
#include <Wire.h>

//Transmit rate in bits per second
#define TRate 7

//assign pins for data lines (lasers)
#define TClock 2
#define TData 3

//
//I2C bus address in hexadecimal. I'm pretty sure it's 39 for both. Check using the port finder (on google drive)
LiquidCrystal_I2C lcd(0x27, 20, 4);

const char *message = "lmao yeet";

void setup() {
  pinMode(TClock, OUTPUT);
  pinMode(TData, OUTPUT);

  //initialise the LCD
  lcd.init();
  lcd.backlight(); //Flashes. Seems to be the only options

  lcd.print("starting tx...");
  delay(500);
  lcd.clear();
  
  lcd.print(message);

  for(int byteC = 0; byteC < strlen(message); byteC ++) {

    char tbyte = message[byteC];

    //clear a line to show each byte
    lcd.noCursor();
    lcd.setCursor(0, 1);
    lcd.print("        ");
    lcd.setCursor(byteC, 0);
    lcd.cursor();

    for (int bitC = 0; bitC < 8; bitC ++) {

      bool tbit = tbyte & (0x80 >> bitC);

      digitalWrite(TData, tbit);
      delay((1000 / TRate) / 2);

      //Write the byte to LCD
      lcd.noCursor();
      lcd.setCursor(0,2);
      lcd.setCursor(bitC, 1);
      lcd.print(tbit ? "1" : "0");
      lcd.setCursor(byteC, 0);
      lcd.cursor();

      //Flash clock 
      digitalWrite(TClock, HIGH);
      delay((1000 / TRate) / 2);
      digitalWrite(TClock, LOW);
      
    }
    
  }

  //ensure the data output is not stuck on hi
  digitalWrite(TData, LOW);
  
}

void loop() {
  // put your main code here, to run repeatedly:

}
