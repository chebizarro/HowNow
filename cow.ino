/* 
 *  Physical Computing Project
 *  Chris Daley
 *  The main Cow module
 */
#include "Storage.h"
#include "WiFi.h"
#include <SD.h>
#include <OneWire.h> 
#include <DallasTemperature.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <ADXL335.h>
#include <TimerOne.h>

// The pin allocations
#define ONE_WIRE_BUS  2 
#define GPS_TX_PIN    3
#define GPS_RX_PIN    4
#define SS_PIN        5
#define ERR_PIN       7
#define WIFI_RX       9
#define WIFI_TX       10
// these are for documentation purposes
#define SD_MOSI       11
#define SD_MISO       12
#define SD_CLK        13

// Some constants
#define TIMER_DELAY   500000
#define GPS_BAUD      4800

// Error "codes"
#define SD_ERROR      3
#define FILE_ERROR    6
#define GPS_ERROR     9

#define WIFI_NAME     "TESCWireless"
#define WIFI_PASSWORD ""
#define REMOTE_HOST   ""

// Globals
OneWire oneWire(ONE_WIRE_BUS); 
DallasTemperature tempSensors(&oneWire);
TinyGPSPlus gps;
SoftwareSerial ss(GPS_RX_PIN, GPS_TX_PIN);
ADXL335 accelerometer;
WiFi wifi(WIFI_RX, WIFI_TX);

int lasterr = 0;
volatile byte sensorFlag;
volatile Data cowData;

void error(int errnum) {
  // blink the error LED errnum times
  for (int i=0; i < errnum; i++) {
    digitalWrite(ERR_PIN, HIGH);
    delay(1000);
    digitalWrite(ERR_PIN, LOW);
    delay(1000);
  }
  lasterr = errnum;
  delay(30000);
}

void setup() {

  Serial.begin(9600);
  
  pinMode(ERR_PIN, OUTPUT);

  // initialize the SD card
  pinMode(SS_PIN, OUTPUT);
 
  if (!SD.begin(SS_PIN)) {
    error(SD_ERROR);
    return;
  }
  
  // set up respiration
  tempSensors.begin(); 

  // set up steps
  accelerometer.begin();

  // set up location
  ss.begin(GPS_BAUD);

  // set up WiFi
  wifi.connect(WIFI_NAME, WIFI_PASSWORD);
  
  // set up timers
  Timer1.initialize(TIMER_DELAY);
  Timer1.attachInterrupt(readSensors);
  
}

void readSensors() {
  
  // get the location
  while (ss.available() > 0) {
    if (gps.encode(ss.read())) {
      if (gps.location.isValid()) {
        cowData.date = gps.date.value();
        cowData.time = gps.time.value();
        cowData.lattitude = gps.location.lat();
        cowData.lattitude = gps.location.lng();
      }
    } else {
      error(GPS_ERROR);
    }
  }

  // get the respiration
  tempSensors.requestTemperatures();
  cowData.respirationRate = tempSensors.getTempCByIndex(0);

  Serial.println(tempSensors.getTempCByIndex(0));
  
  // get the steps
  int x,y,z;
  accelerometer.getXYZ(&x,&y,&z);
  float ax,ay,az;
  accelerometer.getAcceleration(&ax,&ay,&az);

  cowData.steps.x = x;
  cowData.steps.y = y;
  cowData.steps.z = z;
  cowData.steps.ax = ax;
  cowData.steps.ay = ay;
  cowData.steps.az = az;
  
  // there are sensor readings available
  sensorFlag = 1;
}

void loop() {

  // If there is an error, do not log any data
  if (lasterr > 0) {
    error(lasterr);
    return;
  }

  // if there are sensor readings...
  if (sensorFlag ==1) { 
     sensorFlag = 0;

    // create or open the data file
    File dataFile = SD.open("cowdata.jsn", FILE_WRITE);
  
    if(!dataFile) {
      error(FILE_ERROR);
      return;
    }
        
    // save it all
    dataFile << cowData;
  
    // check if there is a network connection
    if(wifi.connected) {
      String dataString = "";
      while (dataFile.available()) {
        dataString += String(dataFile.read());
      }
      wifi.sendData(REMOTE_HOST,"",dataString);
    }

    dataFile.close();
  }
}
