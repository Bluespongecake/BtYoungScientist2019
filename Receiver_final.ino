/*---------------------------------------------------------------------/
    
    This code was made by Max McCormack for the BT young scientist project.

    Schematic of 2 receivers in google drive folder. (32K ohms)
    
    As with the transmitter, this took a long time to make as I did not know
    much about coding, especially in C.

    

/---------------------------------------------------------------------*/

//libraries
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

//define LCD parameters
LiquidCrystal_I2C lcd(0x27, 16, 2);

//define LED indicator pins
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
//clock = A0
//data = A1

void setup() {
  Serial.begin(9600);//for debugging - comment out if unused

  //initiate LCD
  lcd.init();
  lcd.backlight();
  
  //print nice message for users
  
  lcd.print("starting recieve");
  digitalWrite(ClockLED, HIGH);
  digitalWrite(DataLED, HIGH);
  lcd.setCursor(0,1);
  lcd.print("confirm transfer");
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
