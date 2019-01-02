//documentation for shield:https://learn.adafruit.com/adafruit-music-maker-shield-vs1053-mp3-wav-wave-ogg-vorbis-player/library-reference
//documentation for lcd:http://playground.arduino.cc/Code/SerLCD

#include <SoftwareSerial.h>
#include <serLCD.h>

#include <SPI.h>
#include <Adafruit_VS1053.h>
#include <SD.h>



#define BREAKOUT_RESET  9      
#define BREAKOUT_CS     10     
#define BREAKOUT_DCS    8      

#define SHIELD_RESET  -1      
#define SHIELD_CS     7      
#define SHIELD_DCS    6     

#define CARDCS 4     

#define DREQ 3       

Adafruit_VS1053_FilePlayer musicPlayer = 

  Adafruit_VS1053_FilePlayer(SHIELD_RESET, SHIELD_CS, SHIELD_DCS, DREQ, CARDCS);
  
volatile boolean started1 = false;
volatile boolean started2 = false;

int lightSensor = A0;
int irs = A1;

int lcdPin = 2;

serLCD lcd(lcdPin);


void setup() {
  Serial.begin(9600);

  lcd.clear();
  lcd.setBrightness(30);
  lcd.print("la oscuridad nos proteja");

  if (! musicPlayer.begin()) { 
     Serial.println(F("Couldn't find VS1053, do you have the right pins defined?"));
     while (1);
  }
  Serial.println(F("VS1053 found"));
  
  SD.begin(CARDCS);    
  
 
  musicPlayer.setVolume(1,1);


  musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT);  // DREQ int

}
void loop() {
  // put your main code here, to run repeatedly:
  int lightValue = analogRead(lightSensor);
  int distance = getIRS();

  Serial.println(distance);
  delay(100);
  if (lightValue >= 250 && started1 == false) {
    started1 = musicPlayer.startPlayingFile("cut1.mp3"); 
    started2 = false;
    for(int i = 0; i <= 10; i++) {
      lcd.setBrightness(2);
      delay(50);
      lcd.setBrightness(30);
      delay(50);
    }
    lcd.setBrightness(1);
  }
  if (lightValue < 250 && started2 == false) {
    started2 = musicPlayer.startPlayingFile("cut2.mp3"); 
    started1 = false;
    lcd.setBrightness(30);
  }
  if(musicPlayer.playingMusic == false) {
    started1 = false;
    started2 = false; 
  }
  if(distance > 150) {
    lcd.clear();
    musicPlayer.setVolume(20,20);
    lcd.print("ya no se siente tan oscuro");
    
  } else {
    lcd.clear();
    musicPlayer.setVolume(2,2);
    lcd.print("la oscuridad nos proteja");
  }

}

int getIRS() {
  int sensorValue1 = analogRead(irs);
  int sensorValue2 = analogRead(irs);
  int sensorValue3 = ((sensorValue1 + sensorValue2)/2); 
  return sensorValue3;
}





