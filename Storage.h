/*
 * This module uses the ADA254 MicroSD Card
 * the store the monitored data until it can
 * be uploaded
 */
#include <SD.h>

typedef struct Data {
  uint32_t date;
  uint32_t time;
  int heartRate;
  double respirationRate;
  int steps;
  double lattitude;
  double longtitude;
} Data;

void operator<< (File& file, volatile Data& data) {
  file.println("{");
  
  file.print("\"date\":\"");
  file.print(data.date);
  file.println("\",");
  
  file.print("\"time\":\"");
  file.print(data.time);
  file.println("\",");

  file.print("\"heart-rate\":\"");
  file.print(data.heartRate);
  file.println("\",");

  file.print("\"respiration-rate\":\"");
  file.print(data.respirationRate);
  file.println("\",");

  file.print("\"steps\":\"");
  file.print(data.steps);
  file.println("\",");

  file.print("\"lat\":\"");
  file.print(data.lattitude);
  file.println("\",");

  file.print("\"long\":\"");
  file.print(data.longtitude);
  file.println("\"");

  file.println("}");
}


