/*
   -------------------------------------------------------------------------------------
   HX711_ADC
   Arduino library for HX711 24-Bit Analog-to-Digital Converter for Weight Scales
   Olav Kallhovd sept2017
   -------------------------------------------------------------------------------------
*/
/*
   Settling time (number of samples) and data filtering can be adjusted in the config.h file
   For calibration and storing the calibration value in eeprom, see example file "Calibration.ino"
   The update() function checks for new data and starts the next conversion. In order to acheive maximum effective
   sample rate, update() should be called at least as often as the HX711 sample rate; >10Hz@10SPS, >80Hz@80SPS.
   If you have other time consuming code running (i.e. a graphical LCD), consider calling update() from an interrupt routine,
   see example file "Read_1x_load_cell_interrupt_driven.ino".
   This is an example sketch on how to use this library
*/
#include <HX711_ADC.h>
#include <EEPROM.h>
#include <Wire.h>  // Wire Bibliothek hochladen
#include <LiquidCrystal_I2C.h> // Vorher hinzugefügte LiquidCrystal_I2C Bibliothek hochladen
//pins:
const int HX711_dout = 5; //mcu > HX711 dout pin
const int HX711_sck = 4; //mcu > HX711 sck pin
const int TASTER_TARA=7;
const int RELAIS=8;
const int TASTER_SCALE_MODE=9;

const int STATUS_LINE=0;
const int WEIGHT_LINE=1;
const int MODE_LINE=2;
const int calVal_eepromAdress = 0;
const int TIMER=LOW;
const int SCALE=HIGH;

HX711_ADC LoadCell(HX711_dout, HX711_sck);
LiquidCrystal_I2C lcd(0x3F, 20, 4);

long t;
int currentGrindMode=TIMER;
int scaleModePressed = TIMER;

void setup() {
  Serial.begin(57600); delay(10);
  Serial.println("Setup");
  
  initLcd();
  printLine(STATUS_LINE,"Starting...");
  calibrateScale();

 

  pinMode(TASTER_TARA, INPUT);
  pinMode(TASTER_SCALE_MODE, INPUT);
  pinMode(RELAIS, OUTPUT);
   
}
void loop() {
  static boolean newDataReady = 0;
  const int serialPrintInterval = 0; //increase value to slow down serial print activity
  int taraStatus=0;
  scaleModePressed = digitalRead(TASTER_SCALE_MODE);
  
  if(currentGrindMode == scaleModePressed){
    currentGrindMode = TIMER;
    printLine(MODE_LINE, "TIMER MODE");
    delay(120);
  }else if(scaleModePressed == SCALE){
    currentGrindMode = SCALE;
    printLine(MODE_LINE, "SCALE MODE");
    delay(120);
  }
  // check for new data/start next conversion:
  if (LoadCell.update()) newDataReady = true;
  // get smoothed value from the dataset:
  if (newDataReady) {
    if (millis() > t + serialPrintInterval) {
       float weight = LoadCell.getData();
      printLine(WEIGHT_LINE,"Gewicht: "+ String(weight,1) );

      if(weight >= 14 && currentGrindMode == SCALE){
        digitalWrite(RELAIS, HIGH);
        delay(2000);
        weight=0;
      }else{
        digitalWrite(RELAIS, LOW);
      }
      newDataReady = 0;
      t = millis();
    }
  }

  taraStatus=digitalRead(TASTER_TARA); 
    
  if (Serial.available() > 0 ||taraStatus == HIGH) {
    float i;
    char inByte = Serial.read();
    if (inByte == 't'||taraStatus == HIGH) LoadCell.tareNoDelay();
  }
  // check if last tare operation is complete:
  if (LoadCell.getTareStatus() == true) {
    printLine(STATUS_LINE,"Tare complete");
  }
}

void calibrateScale(){
  LoadCell.begin();
  float calibrationValue; // calibration value (see example file "Calibration.ino")
  calibrationValue = 472.78; // uncomment this if you want to set the calibration value in the sketch
  long stabilizingtime = 2000; // preciscion right after power-up can be improved by adding a few seconds of stabilizing time
  boolean _tare = true; //set this to false if you don't want tare to be performed in the next step
  LoadCell.start(stabilizingtime, _tare);
  if (LoadCell.getTareTimeoutFlag()) {
    printLine(STATUS_LINE,"Timeout, check MCU>HX711 wiring and pin designations");
    while (1);
  }
  else {
    LoadCell.setCalFactor(calibrationValue); // set calibration value (float)
    printLine(STATUS_LINE,"Startup is complete");
  }
}

void printLine(int line, String text){
  Serial.println(text);
  lcd.setCursor(0,line);
  if(line == STATUS_LINE){
    lcd.setCursor(0,line); //Text soll beim ersten Zeichen in der ersten Reihe beginnen..
    lcd.print("                    "); //In der ersten Zeile soll der Text „Test Zeile 1“ angezeigt werden    
  } else if(line == WEIGHT_LINE){
  text = text + "     ";
  }
  lcd.setCursor(0,line);
  lcd.print(text); //In der ersten Zeile soll der Text „Test Zeile 1“ angezeigt werden
}

void initLcd(){
  lcd.init(); 
  lcd.backlight();
  lcd.noBlink();
}
