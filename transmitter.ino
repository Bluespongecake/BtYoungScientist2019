/*---------------------------------------------------------------------------------------------------------------------------
           
            30/12/18
            
            Composed by Max Mccormack for the BT young scientitst project 2019
            This will encode a line of text into clock and data that can be transmitted via laser to the receiver module
            It works surprisingly well and fast.
            Unfortunately we had to use relays to control the lasers. Slow? yes. Noisy? very. Stupid? Kind of.
            We reached speeds of 40 bits per second, past this the relays started acting up.
            If I got my hands on some mosfets I'm confident I could have improved the speed significantly - would have 
            required a lot of tweaking on both receiver and transmitter, but achieveable nontheless.
  
  
--------------------------------------------------------------------------------------------------------------------------*/

// this is the library for the controller board "I2C"
#include <Wire.h> 

// pretty self explanatory
#include <LiquidCrystal_I2C.h>

//Hexadecimal I2C bus address. Should be 39. Check back of IIC backpack module for 4 solderpoints that set address
const int I2C = 0x27; 

#define Clock 3
#define Data 4

//---------------------------------------------------------------------------


const char *message = "BT Young Scientist"; // message to be sent


//---------------------------------------------------------------------------

//defining the pins for the LCD (from the I2C (a type of bus))
LiquidCrystal_I2C lcd(I2C,20,4); //20 char 4 rows

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

  //print progress as fraction on lcd
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
      bool tbit = tbyte & (0x80 >> bitc);                          //core line. Took ages to figure out. Goddamit.

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
  //dunno why I left this here. Force of Habit I guess.
}
