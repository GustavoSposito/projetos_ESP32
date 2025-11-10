// BLUETOOTH SLAVE
#include <Arduino.h>
#include <BluetoothSerial.h>
#include <LiquidCrystal_I2C.h>
#include <ArduinoJson.h>

BluetoothSerial BT;
LiquidCrystal_I2C lcd(0x27, 16, 2);

unsigned long previousMillis = 0;
const long interval = 2000; // 2 segundos
String ultimoValor = "";    // Armazena o último valor recebido
JsonDocument doc;
float Cel;
float Far;
float Umi;

void setup()
{
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();

  if (BT.begin("EspSlaveGuGs"))
  {
    Serial.println("Bluetooth iniciado com sucesso");
    Serial.print("Endereço bluetooth ESP: ");
    Serial.println(BT.getBtAddressString());
  }
  else
  {
    Serial.println("Erro ao iniciar o bluetooth");
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Aguardando...");
}

void loop()
{
  unsigned long currentMillis = millis();
  String mensagemRecebida;

  // Recebe dados via Bluetooth
  if (BT.available())
  {
    mensagemRecebida = BT.readStringUntil('\n');
    mensagemRecebida.trim();

    if (mensagemRecebida.length() > 0)
    {
      ultimoValor = mensagemRecebida; // Atualiza o valor armazenado
      // Serial.printf("Mensagem Recebida: %s\n", mensagemRecebida.c_str());
    }
    deserializeJson(doc, mensagemRecebida.c_str());
    Cel = doc["C"];
    Serial.println(Cel);
    Far = doc["F"];
    Serial.println(Cel);
    Umi = doc["U"];
    Serial.println(Cel);
  }


  // Envia dados via Bluetooth a partir do Serial (opcional)
  if (Serial.available())
  {
    String mensagemEnviar = Serial.readStringUntil('\n');
    mensagemEnviar.trim();
    BT.println(mensagemEnviar);
  }

  // Atualiza o LCD a cada 2 segundos
  if (currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis;

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("");
    lcd.setCursor(0, 1);

    if (ultimoValor != "")
    {
      lcd.setCursor(0, 0);
      lcd.print("TempC: ");
      lcd.setCursor(9, 0);
      lcd.print(Cel);

      lcd.setCursor(0, 1);
      lcd.print("Umid: ");
      lcd.setCursor(8, 1);
      lcd.print(Umi);

      lcd.setCursor(0, 2);
      lcd.print("TempF: ");
      lcd.setCursor(9, 2);
      lcd.print(Far);

      lcd.print(" ");
    }
    else
    {
      lcd.print("");
    }
  }
}
