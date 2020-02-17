/*---------------------------------------------------------------------------------------------------------------------------/
    
    30/25/18
    
    This code was made by Max McCormack for the BT young scientist project 2019.
    Recieves clock and data laser signal in two photodiodes.
    Accepts unicode char bytes and displays recieved text on LCD. Maximum 16 chars (limited by LCD size)
    Improvement: refine to use just one laser for both clock and data.
    Schematic of 2 receivers in google drive folder. (32K ohms)

/--------------------------------------------------------------------------------------------------------------------------*/

//libraries
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

//define LCD parameters
LiquidCrystal_I2C lcd(0x27, 16, 2);//make sure I2C address is correct - check on LCD IIC backpack module

//define LED indicator pins - looks cool and good for debugging / testing.
#define ClockLED 12
#define DataLED 13

//define bool, char array,various ints for later use
volatile int ClockRead0 = 0;
volatile int ClockRead1 = 0;

volatile byte rbyte = 0;
volatile int bit_pos = 0;
char message[16];
volatile bool LCDupdate = true;
volatile bool rbit = 0;

//remember
//clock = A0  (LCD)
//data = A1   (LCD)

void setup() {
  //Serial.begin(9600);//for debugging - comment out if unused

  //initiate LCD
  lcd.init();
  lcd.backlight();
  
  //print nice message for users
  
  lcd.print("starting recieve");
  digitalWrite(ClockLED, HIGH);
  digitalWrite(DataLED, HIGH);
  lcd.setCursor(0,1);
  lcd.print("confirm transfer"); //cool startup sequence.
  delay(2000);
  digitalWrite(DataLED, LOW);
  digitalWrite(ClockLED, LOW);
  lcd.clear();
  delay(100);

  strcpy(message, ""); //set message to null
}

void ClockUp(){       //guess when this runs
  Serial.println("1");
  digitalWrite(ClockLED, HIGH);
  int DataRead = analogRead(A1);
  if(DataRead == 0){
    rbit = 1;
  }
  else{
    rbit = 0;
  }

  if (bit_pos == 8) {
    rbyte = 0;
    bit_pos = 0;
  }
  
  if (rbit) {
    rbyte |= (0x80 >> bit_pos);
  }

  bit_pos += 1;

  if (bit_pos == 8) {
    strncat(message, (const char *)&rbyte, 1);
  }
  
  LCDupdate = true;
}

void ClockDown(){
  Serial.println("0");
  digitalWrite(ClockLED, LOW);
}

void loop() {
  int datindicator = analogRead(A1);
  Serial.println(datindicator);
  ClockRead0 = analogRead(A0);
  delay(1);
  if(datindicator == 0){
    digitalWrite(DataLED, HIGH);
  }
  else{
    digitalWrite(DataLED, LOW);
  }
  if((ClockRead1 > 1) & (ClockRead0 == 0)){
    ClockUp();
  }
  else if((ClockRead1 == 0) & (ClockRead0 > 200)){
    ClockDown();
  }
  ClockRead1 = analogRead(A0);
  delay(0.1);  
  if((ClockRead0 > 1) & (ClockRead1 == 0)){
    ClockUp();
  }
  else if((ClockRead0 == 0) & (ClockRead1 > 200)){
    ClockDown();
  }
  if (LCDupdate) {
    LCDupdate = false;
    
    lcd.noCursor();
    lcd.setCursor(0, 0);
    lcd.print(message);
    lcd.setCursor(0, 1);
    for (int i = 0; i < 8; i += 1) {
      if (i < bit_pos) {
        lcd.print(rbyte & (0x80 >> i) ? "1" : "0");
      } else {
        lcd.print(" ");
      }
    }
    lcd.setCursor(strlen(message), 0);
    lcd.cursor();
  }
}
