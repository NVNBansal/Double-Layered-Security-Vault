#include <Adafruit_Fingerprint.h>
#include <Keypad.h>

SoftwareSerial mySerial(11, 12);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

#define RELAY_PIN       13
#define ACCESS_DELAY    3000 // Keep lock unlocked for 3 seconds



char * password = "2369"; // To increase the passcode length change the numerical to the size after position

int position = 0; 

const byte ROWS = 4; // 4 rows
const byte COLS = 4; // 4 columns 


char keys[ROWS][COLS] =
{
{'1','2','3','A'},
{'4','5','6','B'},
{'7','8','9','C'},
{'*','0','#','D'}
}; //mapping of the keys done w.r.t to the grid keypad

byte rowPins[ROWS] = {9, 8, 7, 6}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {5, 4, 3, 2}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );




void setup()
{
  // set the data rate for the sensor serial port
  finger.begin(57600);
  delay(5);
  if (finger.verifyPassword()) 
  {
  } 
  else 
  {
    while (1) { delay(1); }
  }
  
  pinMode(RELAY_PIN, OUTPUT);
  LockedPosition(true);
  digitalWrite(RELAY_PIN, HIGH);   //Switch off relay initially. Relay is LOW level triggered relay so we need to write HIGH.
}

void loop()
{
  if ( getFingerPrint() != -1)
  {
    digitalWrite(RELAY_PIN, LOW);
    delay(ACCESS_DELAY);
    digitalWrite(RELAY_PIN, HIGH);   
  }  
  delay(50);            //Add some delay before next scan.


  
char key = keypad.getKey();
if (key == '*' || key == 'A') //OR operator used to lock the device back again 
{
position = 0;
LockedPosition(true);
}
if (key == password[position])
{
position ++;
}
if (position == 4) // change this if you want to increase the password length
{
LockedPosition(false);
}
delay(100);
}
void LockedPosition(int locked)
{
if (locked)
{
digitalWrite(RELAY_PIN, LOW);
}
else
{
digitalWrite(RELAY_PIN, HIGH);
}
}

// returns -1 if failed, otherwise returns ID #
int getFingerPrint() 
{
  int p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;

  // found a match!
  return finger.fingerID;
}
