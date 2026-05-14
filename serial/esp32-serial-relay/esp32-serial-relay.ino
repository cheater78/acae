// UART pins for external device
#define RXD2 16
#define TXD2 17

// Baud rates
#define PC_BAUD 115200
#define DEVICE_BAUD 115200

// rx -> from dev, tx -> to dev
#define BUFFER_SIZE 1024U
unsigned char rx_buf[BUFFER_SIZE] = { 0 };
unsigned char tx_buf[BUFFER_SIZE] = { 0 };

void setup() {
  Serial.begin(PC_BAUD);
  Serial2.begin(DEVICE_BAUD, SERIAL_8N1, RXD2, TXD2);
  Serial.println("ESP32 UART bridge ready");
}

void loop() {
  {
    unsigned int length = Serial2.available();
    if (length) {
      length = Serial2.readBytes(rx_buf, min(length, BUFFER_SIZE));
      Serial.write(rx_buf, length);
    }
  }

  {
    unsigned int length = Serial.available();
    if (length) {
      length = Serial.readBytes(tx_buf, min(length, BUFFER_SIZE));
      Serial2.write(tx_buf, length);
    }
  }
}