#define SENSOR_PIR 33

void setup() {
  Serial.begin(115200);
  pinMode(SENSOR_PIR, INPUT);

}

void loop() {
  
  int status = digitalRead(SENSOR_PIR);
  
  if(status == HIGH){
    Serial.println("Detectado movimento");
 } else{
    Serial.println("Nenhum movimento");
 }
 delay(1000);
}
