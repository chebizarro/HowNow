/*
 * This module uses the ADA254 MicroSD Card
 * to store the monitored data until it can
 * be uploaded
 */
#include <SD.h>

typedef struct Step {
  int x, y, z;
  float ax,ay,az;
} Step;

typedef struct Data {
  uint32_t date;
  uint32_t time;
  double respirationRate;
  Step steps;
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

  file.print("\"respiration-rate\":\"");
  file.print(data.respirationRate);
  file.println("\",");

  file.print("\"steps\": {");
  file.print("\"x\":\"");
  file.print(data.steps.x);
  file.println("\",");
  file.print("\"x\":\"");
  file.print(data.steps.y);
  file.println("\",");
  file.print("\"x\":\"");
  file.print(data.steps.z);
  file.println("\",");
  file.print("\"x\":\"");
  file.print(data.steps.ax);
  file.println("\",");
  file.print("\"x\":\"");
  file.print(data.steps.az);
  file.println("\",");
  file.print("\"x\":\"");
  file.print(data.steps.ay);
  file.println("\"");
  file.println("},");

  file.print("\"lat\":\"");
  file.print(data.lattitude);
  file.println("\",");

  file.print("\"long\":\"");
  file.print(data.longtitude);
  file.println("\"");

  file.println("}");
}


