#include "SPIFFS.h"
#include "FS.h"
#include <Adafruit_SSD1306.h> 

#define PIR_PIN 33

bool pirState = false;
File logFile;
const uint8_t WIDTH = 128;
const uint8_t HEIGHT = 64;
int n = 0;

Adafruit_SSD1306 display(WIDTH, HEIGHT, &Wire, -1); 
void setup(){
  Serial.begin(115200);

  if(!SPIFFS.begin(true)){
    Serial.println("Erro ao montar o sistema de arquivos SPIFFS");
    return;
  }

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)){
    Serial.println(F("SSD1306 erro de alocation"));
    for(;;);
  }

  delay(2000);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Digite:");
  display.println("- 1 Exibir log");
  display.println("- 2 Limpar log");
  display.display();

  pinMode(PIR_PIN, INPUT);

  logFile = SPIFFS.open("/log.txt", "w");
  logFile.println("Log Iniciado");
  logFile.close();
}

void loop(){
  if(digitalRead(PIR_PIN) == HIGH && !pirState){
    pirState = true;
    logDetection();
  }else if(digitalRead(PIR_PIN) == LOW && pirState){
    pirState = false;
  }

  if(Serial.available()){
    char input = Serial.read();
    if(input == '1'){
      printLogFile();
    }else if(input == '2'){
      clearLogFile();
    }
  }
}

void logDetection(){
  n+=1;
  Serial.println("Movimento Detectado: ");
 // Serial.print(millis());

  File logFile = SPIFFS.open("/log.txt", "w");
  if(!logFile){
    Serial.println("Failed to open log file");
    return;
  }
  logFile.print("Detectado: ");
  logFile.print(n);
  logFile.println(" vezes");
  logFile.println("---------------------");
  logFile.close();
}

void printLogFile(){
  logFile = SPIFFS.open("/log.txt", "r");
  while(logFile.available()){
    Serial.write(logFile.read());
  }
  logFile.close();
}

void clearLogFile(){
  SPIFFS.remove("/log.txt");
  logFile = SPIFFS.open("/log.txt", "w");
  logFile.println("Log Iniciado");
  logFile.close();
  Serial.println("Arquivo limpo com sucesso!");
  n = 0;
}
