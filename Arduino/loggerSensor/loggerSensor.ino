#include <FS.h>
#include <SPIFFS.h>
char x = 32;

const int PIR_PIN = 33;
const char* FILE_NAME = "/motion_log.txt";

void setup() {

  pinMode(PIR_PIN, INPUT);
  Serial.begin(115200);

  if(!SPIFFS.begin(true)) {
    Serial.println("error occured while mounting SPIFFS");
    return;
  }

  Serial.println("Type '1' to read the log");
}

void logMotion() {
  
   Serial.println("Motion detected");
   Serial.println("Type '1' to read the log");
   Serial.println(x);
   x++;

   File logFile = SPIFFS.open(FILE_NAME, FILE_APPEND);

   if(!logFile) {
      Serial.println("Failed to open log file");
      return;
   }

   logFile.println("Motion detected");
   logFile.println("-----------------");
   logFile.close();
   
}

void readLogFile() {
  
   Serial.println("Reading log file");
   File logFile = SPIFFS.open(FILE_NAME);

   if(!logFile) {
      Serial.println("Failed to open log file");
      return;
   }

   while(logFile.available()) {
    Serial.write(logFile.read());
   }
   logFile.close();
}

void loop() {
  
  int motion = digitalRead(PIR_PIN);
  
  if(motion == HIGH) {
    logMotion();
  }
  
  if(Serial.available() > 0) {
    char option = Serial.read();
    if(option == '1') {
      readLogFile();
    }
  }
  delay(1000);
}
