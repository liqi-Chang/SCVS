//https://dl.espressif.com/dl/package_esp32_index.json

#include <esp_now.h>
#include <WiFi.h>

#include <Servo.h> 

int a = 12;
int b = 14;
int c = 27;
int d = 26;

Servo myservo;
int servoPin=16;

typedef struct struct_message {
  int pwmx;
  int pwmy;

} struct_message;
struct_message myData;


void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));

}

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  myservo.attach(servoPin);
  myservo.write(85);
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);

  pinMode(12, OUTPUT);
  pinMode(14, OUTPUT);
  pinMode(27, OUTPUT);
  pinMode(26, OUTPUT);
}
 
void loop() {
if ((myData.pwmy)<=210){
    myservo.write(65);
  }
  else if((myData.pwmy)>=260){
    myservo.write(105);
  }
  else{
    myservo.write(85);
  }


  if((myData.pwmx)<=210){
    digitalWrite(14,HIGH);
    digitalWrite(26,HIGH);
    digitalWrite(12,LOW);
    digitalWrite(27,LOW);
  }
  else if((myData.pwmx)>=260){
    digitalWrite(12,HIGH);
    digitalWrite(27,HIGH);
    digitalWrite(14,LOW);
    digitalWrite(26,LOW);      
  }
  else{
    digitalWrite(12,LOW);
    digitalWrite(14,LOW);
    digitalWrite(27,LOW);
    digitalWrite(26,LOW);
  }
  Serial.print("PWMx : ");
  Serial.println(myData.pwmx);

  Serial.print("PWMy : ");
  Serial.println(myData.pwmy);

}