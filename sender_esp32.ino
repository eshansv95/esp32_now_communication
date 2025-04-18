#include <WiFi.h>
#include <esp_now.h>

// Replace with the actual MAC address of the receiver
uint8_t receiverMAC[] = { 0xF4, 0x65, 0x0B, 0x4A, 0x42, 0x48 };

void setup() {
  Serial.begin(115200);

  // Initialize Wi-Fi in station mode
  WiFi.mode(WIFI_STA);

  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Set up peer information
  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, receiverMAC, 6);
  peerInfo.channel = 0; // Use default channel (must match receiver)
  peerInfo.encrypt = false;

  // Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

  Serial.println("Sender is ready.");
}

void loop() {
  const char *message = "55"; // Test message

  // Send message
  esp_err_t result = esp_now_send(receiverMAC, (uint8_t *)message, strlen(message));

  if (result == ESP_OK) {
    Serial.println("Message sent successfully");
  } else {
    Serial.println("Error sending message");
  }

  delay(2000); // Wait 2 seconds
}
