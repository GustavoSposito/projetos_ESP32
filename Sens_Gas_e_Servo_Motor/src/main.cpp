#include <Arduino.h>
#include "BluetoothSerial.h"
#include <ESP32Servo.h>

BluetoothSerial BT;

const int ledPin = 2;
const int servoPin = 15;

Servo servoMotor;

int angulo = 0;   // Para girar continuamente

void setup() {
  Serial.begin(9600);
  
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  servoMotor.attach(servoPin);
  servoMotor.write(0);

  BT.begin("ESP32_RECEBIDOR");
  Serial.println("Aguardando dados do MASTER...");
  Serial.println(BT.getBtAddressString());

}

void loop() {

  // Verifica se recebeu mensagem do MASTER
  if (BT.available()) {
    String msg = BT.readStringUntil('\n');
    msg.trim();

    Serial.print("Recebido: ");
    Serial.println(msg);

    // Verifica se é um alerta
    if (msg.startsWith("ALERTA!")) {

      // -------------------------
      // LED ACESO
      // -------------------------
      digitalWrite(ledPin, HIGH);

      // -------------------------
      // SERVO GIRANDO CONTINUAMENTE
      // -------------------------
      angulo += 10;
      if (angulo > 180) angulo = 0;
      servoMotor.write(angulo);

      Serial.println(">>> GAS ALTO → LED ON / SERVO GIRANDO");

    } else {

      // -------------------------
      // LED APAGADO
      // -------------------------
      digitalWrite(ledPin, LOW);

      // -------------------------
      // PARA O SERVO
      // -------------------------
      servoMotor.write(0);

      Serial.println(">>> GAS NORMAL → LED OFF / SERVO PARADO");
    }
  }

  delay(200);
}