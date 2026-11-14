/***************************************************************************
  This is a library for the BMP3XX temperature & pressure sensor

  Designed specifically to work with the Adafruit BMP388 Breakout
  ----> http://www.adafruit.com/products/3966

  These sensors use I2C or SPI to communicate, 2 or 4 pins are required
  to interface.

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing products
  from Adafruit!

  Written by Limor Fried & Kevin Townsend for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ***************************************************************************/

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BMP3XX.h"
#include <SD.h>

#define BMP_SCK 13
#define BMP_MISO 12
#define BMP_MOSI 11
#define BMP_CS 10

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BMP3XX bmp;

//SD stuff from example script
const int chipSelect = 9;
File myFile;

//initialize variables
double temperature, pressure, altitude;
long int time;


void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.println("why");
  Serial.println("Adafruit BMP388 / BMP390 test");
  Serial.println("why not");
  if (!bmp.begin_I2C()) {   // hardware I2C mode, can pass in address & alt Wire
  //if (! bmp.begin_SPI(BMP_CS)) {  // hardware SPI mode  
  //if (! bmp.begin_SPI(BMP_CS, BMP_SCK, BMP_MISO, BMP_MOSI)) {  // software SPI mode
    Serial.println("aaaaabbbbbb");
    
    while (1);
  }
  Serial.println("aaaaa");
  // Set up oversampling and filter initialization
  bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
  bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
  bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
  bmp.setOutputDataRate(BMP3_ODR_50_HZ);

  //End of altimeter initialization
  Serial.println("Altimeter initialized");


  Serial.print("Initializing SD card...");

   if (!SD.begin(chipSelect)) {
    Serial.println("initialization failed. Things to check:");
    Serial.println("1. is a card inserted?");
    Serial.println("2. is your wiring correct?");
    Serial.println("3. did you change the chipSelect pin to match your shield or module?");
    Serial.println("Note: press reset button on the board and reopen this Serial Monitor after fixing your issue!");
    while (true);
  }


  Serial.println("initialization done.");



}



void loop() {
  
  time = millis();
  if (! bmp.performReading()) {
    Serial.println("Failed to perform reading :(");
    return;
  }
  Serial.print("Temperature = ");
  temperature = bmp.readTemperature();
  Serial.print(temperature);
  Serial.println(" *C");

  Serial.print("Pressure = ");
  pressure = bmp.pressure / 100.0;
  Serial.print(pressure);
  Serial.println(" hPa");

  Serial.print("Approx. Altitude = ");
  altitude = bmp.readAltitude(SEALEVELPRESSURE_HPA);
  Serial.print(altitude);
  Serial.println(" m");

  Serial.println();
  Serial.println();

  Serial.print(time+' , '+altitude);

  myFile = SD.open("log.txt", FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Writing to datalog...");
    myFile.print(time);
    myFile.print(',');
    myFile.println(altitude);
    // close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }


  Serial.println();
  delay(100);
}