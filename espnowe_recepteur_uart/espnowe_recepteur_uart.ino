#include <esp_now.h>
#include <WiFi.h>

// Définition des broches UART
#define UART_TX_PIN 33  // TX
#define UART_RX_PIN 34  // RX
#define UART_BAUD    115200

// Buffer pour stocker le message extrait
String lastMessage = "";
unsigned long lastSendTime = 0;
const unsigned long sendInterval = 500; // 500 ms

void OnDataRecv(const esp_now_recv_info_t* info, const uint8_t* data, int len) {
  // Convertir les données reçues en String
  String received = "";
  for (int i = 0; i < len; i++) {
    received += (char)data[i];
  }

  // Rechercher les délimiteurs '<' et '>'
  int startIdx = received.indexOf('<');
  int endIdx = received.indexOf('>');

  if (startIdx != -1 && endIdx != -1 && endIdx > startIdx) {
    // Extraire le contenu entre '<' et '>'
    lastMessage = received.substring(startIdx + 1, endIdx);
    Serial.print("Message reçu : ");
    Serial.println(lastMessage);
  } else {
    Serial.println("Trame invalide reçue.");
  }
}

void setup() {
  Serial.begin(115200);
  delay(100);

  // Initialiser UART sur les broches définies
  Serial1.begin(UART_BAUD, SERIAL_8N1, UART_RX_PIN, UART_TX_PIN);
  Serial.println("UART initialisé sur GPIO 33 (TX) et GPIO 34 (RX)");

  // Initialiser ESP-NOW
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Erreur lors de l'initialisation d'ESP-NOW");
    return;
  }
  esp_now_register_recv_cb(OnDataRecv);
  Serial.println("ESP-NOW initialisé et en attente de trames...");
}

void loop() {
  // Vérifier si 500 ms se sont écoulées et si un message est disponible
  if (millis() - lastSendTime >= sendInterval && lastMessage.length() > 0) {
    // Envoyer le message via UART
    Serial1.println(lastMessage);
    Serial.print("Message envoyé via UART : ");
    Serial.println(lastMessage);
    lastSendTime = millis();
  }
}
