/* ============================================================
   Code 1.3.3 : Emergency Button (Node) Product Firmware
   Features  : TTL + Deduplication + Random Delay + Wake-on-Radio
   Board     : NodeMCU ESP32 CH340 (ESP-WROOM-32)
   Module    : AS32-TTL-100

   Behavior:
   - Node stays in AS32 Power-Saving mode (sleep-monitor)
   - Wakes automatically on RF activity
   - Rebroadcasts using Wake-Up mode
   - Returns to Power-Saving mode

   ============================================================ */

#include <Arduino.h>
#include "BluetoothSerial.h"

/* ================= PINOUT (SHARED – DO NOT CHANGE) =================
   ESP32 PIN   →  AS32 PIN
   D17 (GPIO17) → RXD
   D16 (GPIO16) → TXD
   D25 (GPIO25) → MD0
   D26 (GPIO26) → MD1
   ================================================================= */

#define LORA_TX   17   // D17 → AS32 RXD
#define LORA_RX   16   // D16 → AS32 TXD
#define LORA_MD0  25   // D25 → MD0
#define LORA_MD1  26   // D26 → MD1

HardwareSerial LoRaSerial(2);
BluetoothSerial SerialBT;

/* ================= NODE CONFIG ================= */
const String NODE_NUMBER = "Node01";   // CHANGE PER NODE
uint8_t seqCounter = 1;
const int DEFAULT_TTL = 1;

/* ================= BUFFERS ================= */
String rxBuffer = "";

/* ================= DEDUP STORAGE ================= */
String dedupList[10];
uint8_t dedupIndex = 0;

/* ================= MODE CONTROL ================= */

// AS32 Power-Saving Mode (Sleep + RF Monitor)
void setPowerSavingMode() {
  digitalWrite(LORA_MD0, HIGH); // MD0 = 1
  digitalWrite(LORA_MD1, LOW);  // MD1 = 0
  delay(5);
}

// AS32 Wake-Up Transmit Mode
void setWakeUpMode() {
  digitalWrite(LORA_MD0, LOW);  // MD0 = 0
  digitalWrite(LORA_MD1, HIGH); // MD1 = 1
  delay(5);
}

/* ================= DEDUP FUNCTIONS ================= */

bool isDuplicate(String id) {
  for (int i = 0; i < 10; i++) {
    if (dedupList[i] == id) return true;
  }
  return false;
}

void storeDedupID(String id) {
  dedupList[dedupIndex] = id;
  dedupIndex = (dedupIndex + 1) % 10;
}

/* ================= SETUP ================= */

void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(LORA_MD0, OUTPUT);
  pinMode(LORA_MD1, OUTPUT);

  LoRaSerial.begin(9600, SERIAL_8N1, LORA_RX, LORA_TX);

  SerialBT.begin("ESP32_BT_MONITOR");
  randomSeed(micros());

  // Start in low-power listening mode
  setPowerSavingMode();

  Serial.println("=== Code 1.3.3 : Emergency Node (Wake-on-Radio) ===");
}

/* ================= LOOP ================= */

void loop() {

  /* ============================================================
     LORA RECEIVE (AUTO WAKE) + TTL + DEDUP + REBROADCAST
     ============================================================ */

  while (LoRaSerial.available()) {
    char c = LoRaSerial.read();

    if (c == '\n') {
      Serial.print("[Received] ");
      Serial.println(rxBuffer);

      int p1 = rxBuffer.indexOf('|');
      int p2 = rxBuffer.indexOf('|', p1 + 1);
      int p3 = rxBuffer.indexOf('|', p2 + 1);

      if (p1 > 0 && p2 > p1 && p3 > p2) {

        String nodeNo = rxBuffer.substring(0, p1);
        String seqNo  = rxBuffer.substring(p1 + 1, p2);
        int ttl       = rxBuffer.substring(p2 + 1, p3).toInt();
        String msg    = rxBuffer.substring(p3 + 1);

        if (ttl <= 0) {
          Serial.println("[Dropped] TTL expired");
          rxBuffer = "";
          return;
        }

        String dedupID = nodeNo + seqNo;
        if (isDuplicate(dedupID)) {
          Serial.println("[Dropped] Duplicate");
          rxBuffer = "";
          return;
        }

        storeDedupID(dedupID);

        int randomDelayMs = random(1000, 5001);
        Serial.print("[Rebroadcast Delay] ");
        Serial.print(randomDelayMs);
        Serial.println(" ms");
        delay(randomDelayMs);

        // Wake-Up transmit
        setWakeUpMode();

        String rebroadcastMsg =
          nodeNo + "|" +
          seqNo + "|" +
          String(ttl - 1) + "|" +
          msg + "\n";

        LoRaSerial.write(rebroadcastMsg.c_str(), rebroadcastMsg.length());

        Serial.print("[Rebroadcasted] ");
        Serial.print(rebroadcastMsg);

        // Return to low-power listening
        setPowerSavingMode();
      }

      rxBuffer = "";
    } else {
      rxBuffer += c;
    }
  }
}
