//https://dl.espressif.com/dl/package_esp32_index.json

#include <esp_now.h>
#include <WiFi.h>

// 宣告變數
int valx;
int valy;
int vall;
int valr;

uint8_t broadcastAddress[] = {0xEB, 0x6B, 0xEA, 0xC9, 0x9D, 0x1C}; // 替換為你的接收端MAC地址

// 定義結構體來保存數據
typedef struct struct_message {
  int pwmx;
  int pwmy;
} struct_message;

struct_message myData;

esp_now_peer_info_t peerInfo;

// 發送數據後的回調函數
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
  // 初始化串口監視器
  Serial.begin(115200);

  // 設置WiFi模式為STA模式
  WiFi.mode(WIFI_STA);

  // 初始化引腳
  pinMode(36, INPUT);
  pinMode(39, INPUT);
  pinMode(25, INPUT);
  pinMode(26, INPUT);

  // 初始化ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // 註冊發送回調
  esp_now_register_send_cb(OnDataSent);

  // 添加對等體
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

  analogReadResolution(9);
}

// 讀取模擬輸入的平均值函數
int readAverage(int pin, int numSamples) {
  long sum = 0;
  for (int i = 0; i < numSamples; i++) {
    sum += analogRead(pin);
    delay(5); // 添加短暫延時減少干擾
  }
  return sum / numSamples;
}

void loop() {
  // 讀取X軸數據
  valx = readAverage(36, 10);
  //valx = map(valx, 0, 4095, 0, 511);
  myData.pwmx = valx;
  Serial.print("x: ");
  Serial.println(valx);

  // 讀取Y軸數據
  valy = readAverage(39, 10);
  //valy = map(valy, 0, 4095, 0, 511);
  myData.pwmy = valy;
  Serial.print("y: ");
  Serial.println(valy);

  // 發送數據
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));

  delay(100); // 延時100毫秒
}
