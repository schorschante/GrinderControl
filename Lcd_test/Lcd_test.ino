#include <Wire.h>  // Wire Bibliothek hochladen
#include <LiquidCrystal_I2C.h> // Vorher hinzugefügte LiquidCrystal_I2C Bibliothek hochladen
LiquidCrystal_I2C lcd(0x3F, 20, 4);  //Hier wird das Display benannt (Adresse/Zeichen pro Zeile/Anzahl Zeilen). In unserem Fall „lcd“. Die Adresse des I²C Displays kann je nach Modul variieren.

void setup()
{
lcd.init(); //Im Setup wird der LCD gestartet
lcd.backlight(); //Hintergrundbeleuchtung einschalten (0 schaltet die Beleuchtung aus).
}

void loop()
{
lcd.setCursor(0,0); //Text soll beim ersten Zeichen in der ersten Reihe beginnen..
lcd.print("Test Zeile 1"); //In der ersten Zeile soll der Text „Test Zeile 1“ angezeigt werden
lcd.setCursor(0,1); //Genauso geht es bei den weiteren drei Zeilen weiter
lcd.print("Test Zeile 2");
lcd.setCursor(0,2);
lcd.print("Test Zeile 3");
lcd.setCursor(0,3);
lcd.print("Test Zeile 4");
}
