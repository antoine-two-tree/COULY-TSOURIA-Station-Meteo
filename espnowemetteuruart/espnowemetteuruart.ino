#include <esp_now.h>
#include <WiFi.h>

// Définition des pins UART et de la vitesse
#define UART_RX_PIN 33
#define UART_TX_PIN 34
#define UART_BAUD    115200

// Taille max d’un message ESP-NOW (<= 250 octets)
#define MAX_MSG_LEN  250

// Adresse broadcast pour tous les peers
uint8_t broadcastAddr[] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};

// Buffer de réception UART
uint8_t uartBuf[MAX_MSG_LEN];
size_t  uartLen = 0;

// Callback après envoi ESP-NOW
void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("ESP-NOW send status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "SUCCESS" : "FAIL");
}

void setup() {
  Serial.begin(115200);
  delay(100);

  // Initialisation du port UART matériel 1 sur GPIO 33/34
  Serial1.begin(UART_BAUD, SERIAL_8N1, UART_RX_PIN, UART_TX_PIN);
  Serial.println("Serial1 initialisée sur RX=33, TX=34");

  // Mode station pour ESP-NOW
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Erreur d'initialisation ESP-NOW");
    while (true) { delay(1000); }
  }
  esp_now_register_send_cb(onDataSent);

  // Configuration du peer broadcast
  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, broadcastAddr, 6);
  peerInfo.channel = 0;      // canal actuel
  peerInfo.encrypt = false;  // pas de chiffrement
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Échec ajout du peer broadcast");
    while (true) { delay(1000); }
  }

  Serial.println("Setup terminé, en attente de données UART...");
}

void loop() {
  // Lecture non bloquante de la liaison UART
  while (Serial1.available() && uartLen < MAX_MSG_LEN) {
    int c = Serial1.read();
    uartBuf[uartLen++] = (uint8_t)c;
    // Dès qu'on détecte un '\n', on envoie
    if (c == '\n') {
      // Envoi via ESP-NOW
      esp_err_t res = esp_now_send(broadcastAddr, uartBuf, uartLen);
      if (res != ESP_OK) {
        Serial.printf("Erreur esp_now_send: %d\n", res);
      }
      // Réinitialisation du buffer
      uartLen = 0;
    }
  }

  // Si le buffer est plein sans newline, on l'envoie malgré tout
  if (uartLen == MAX_MSG_LEN) {
    esp_now_send(broadcastAddr, uartBuf, uartLen);
    uartLen = 0;
  }
}
