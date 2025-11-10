// BLUETOOTH SLAVE
#include <Arduino.h>
#include <BluetoothSerial.h>
#include <LiquidCrystal_I2C.h>

BluetoothSerial BT;
LiquidCrystal_I2C lcd(0x27, 16, 2);

unsigned long previousMillis = 0;
const long interval = 2000; // 2 segundos
String ultimoValor = ""; // Armazena o último valor recebido

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();

  if (BT.begin("EspSlaveGuGs")) {
    Serial.println("Bluetooth iniciado com sucesso");
    Serial.print("Endereço bluetooth ESP: ");
    Serial.println(BT.getBtAddressString());
  } else {
    Serial.println("Erro ao iniciar o bluetooth");
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Aguardando...");
}

void loop() {
  unsigned long currentMillis = millis();

  // Recebe dados via Bluetooth
  if (BT.available()) {
    String mensagemRecebida = BT.readStringUntil('\n');
    mensagemRecebida.trim();
    
    if (mensagemRecebida.length() > 0) {
      ultimoValor = mensagemRecebida; // Atualiza o valor armazenado
      Serial.printf("Mensagem Recebida: %s\n", mensagemRecebida.c_str());
    }
  }
 
  // Envia dados via Bluetooth a partir do Serial (opcional)
  if (Serial.available()) {
    String mensagemEnviar = Serial.readStringUntil('\n');
    mensagemEnviar.trim();
    BT.println(mensagemEnviar);
  }

  // Atualiza o LCD a cada 2 segundos
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("");
    lcd.setCursor(0, 1);

    if (ultimoValor != "") {
      lcd.print(ultimoValor);
      lcd.print(" ");
    } else {
      lcd.print("");
    }
  }
}
