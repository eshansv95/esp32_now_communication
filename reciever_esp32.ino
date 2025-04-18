#include <WiFi.h>
#include <esp_now.h>

// Receiver MAC address (updated with the provided MAC address)
uint8_t receiverMAC[] = { 0xF4, 0x65, 0x0B, 0x4A, 0x42, 0x48 };

#define LED_PIN 17
void blinkLED() {
  for (int i = 0; i < 5; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(500);
    digitalWrite(LED_PIN, LOW);
    delay(500);
  }
}

void OnDataRecv(const uint8_t *mac, const uint8_t *data, int len) {

  String recv ="";
  
  Serial.println("Data received: ");
  for (int i = 0; i < len; i++) {
    Serial.print((char)data[i]);
    char c = (char)data[i];
    recv += c;
    
  }
  if(recv.toInt() == 55){
    Serial.println("led should blink");
    blinkLED();
  }
  
  Serial.println();
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  // Initialize Wi-Fi
  WiFi.mode(WIFI_STA);
  
  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register the callback function to receive data
  esp_now_register_recv_cb(OnDataRecv);

  // Add the sender device to the peer list using the MAC address
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, receiverMAC, 6);
  peerInfo.channel = 0;  // use the current channel
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

  Serial.println("Receiver is ready.");
}

void loop() {
  // The receiver is always listening
}
