#include <Arduino.h>
#include <WiFi.h>
#include <LiquidCrystal_I2C.h>
#include <ezTime.h>

// -------------------- CONFIG WIFI --------------------
const char* wifiSSID = "SENAI IoT";   // <-- Troque pelo nome da sua rede
const char* wifiSenha = "Senai@IoT"; // <-- Troque pela senha da sua rede

// -------------------- LCD I2C --------------------
LiquidCrystal_I2C lcd(0x27, 16, 2);

// -------------------- LED e Botão --------------------
#define PINO_LED 2
#define PINO_BOTAO 4

// -------------------- EZTIME --------------------
Timezone FusoBrasil;

// -------------------- Variáveis do despertador --------------------
int horaDespertador = -1;
int minutoDespertador = -1;
bool despertadorAtivo = false;
bool despertadorDisparado = false;

// -------------------- Controle do LCD --------------------
String ultimaLinha1 = "";
String ultimaLinha2 = "";

// -------------------- Conectar ao WiFi --------------------
void conectarWiFi() {
  Serial.print("Conectando ao WiFi...");
  WiFi.begin(wifiSSID, wifiSenha);

  int tentativas = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    tentativas++;
    if (tentativas > 30) {
      Serial.println("\nERRO: Não conectou ao WiFi!");
      Serial.print("Status WiFi: ");
      Serial.println(WiFi.status());
      while (1); // trava para debug
    }
  }

  Serial.println("\nWiFi conectado!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

// -------------------- Solicitar horário do despertador --------------------
void solicitarHorarioDespertador() {
  Serial.println("\nDigite o horário do despertador (HH:MM):");

  while (!Serial.available()) delay(10);

  String entrada = Serial.readStringUntil('\n');
  entrada.trim();

  if (entrada.length() == 5 && entrada.charAt(2) == ':') {
    horaDespertador   = entrada.substring(0, 2).toInt();
    minutoDespertador = entrada.substring(3, 5).toInt();
    despertadorAtivo = true;
    Serial.println("Despertador configurado para " + entrada);
  } else {
    Serial.println("Formato inválido!");
  }
}

// -------------------- Atualiza LCD somente quando necessário --------------------
void atualizarLCD() {
  String dataHora = FusoBrasil.dateTime("d/m/Y H:i:s");

  String linha1 = dataHora;  // ex: 03/02/2025 14:21:56

  String linha2;
  if (horaDespertador >= 0) {
    char buf[32];
    sprintf(buf, "Alarme %02d:%02d %s",
            horaDespertador, minutoDespertador,
            despertadorAtivo ? "ON" : "OFF");
    linha2 = String(buf);
  } else {
    linha2 = "Alarme --:--";
  }

  if (linha1 != ultimaLinha1) {
    lcd.setCursor(0, 0);
    lcd.print("                ");
    lcd.setCursor(0, 0);
    lcd.print(linha1);
    ultimaLinha1 = linha1;
  }

  if (linha2 != ultimaLinha2) {
    lcd.setCursor(0, 1);
    lcd.print("                ");
    lcd.setCursor(0, 1);
    lcd.print(linha2);
    ultimaLinha2 = linha2;
  }
}

// -------------------- SETUP --------------------
void setup() {
  Serial.begin(115200);
  delay(500);

  lcd.init();
  lcd.backlight();

  pinMode(PINO_LED, OUTPUT);
  pinMode(PINO_BOTAO, INPUT_PULLUP);

  // Conectar ao Wi-Fi
  conectarWiFi();

  // Inicializa ezTime e define fuso horário
  setServer("pool.ntp.org");
  Serial.println("Sincronizando horário...");
  waitForSync();
  FusoBrasil.setLocation("America/Sao_Paulo");
  Serial.println("Hora sincronizada!");
  Serial.println("Data/Hora atual: " + FusoBrasil.dateTime());

  // Solicitar horário do despertador
  solicitarHorarioDespertador();
}

// -------------------- LOOP --------------------
void loop() {
  events(); // ezTime mantém o tempo atualizado

  // Verifica disparo do alarme
  int h = FusoBrasil.hour();
  int m = FusoBrasil.minute();
  int s = FusoBrasil.second();

  if (despertadorAtivo && !despertadorDisparado &&
      h == horaDespertador &&
      m == minutoDespertador &&
      s == 0) {
    despertadorDisparado = true;
    Serial.println("DESPERTADOR DISPAROU!");
  }

  // --- Alarme ativo: piscar LED e LCD ---
  if (despertadorDisparado) {
    digitalWrite(PINO_LED, millis() % 500 < 250);

    if (millis() % 600 < 300) lcd.backlight();
    else lcd.noBacklight();

    // Botão para desligar
    if (digitalRead(PINO_BOTAO) == LOW) {
      despertadorDisparado = false;
      despertadorAtivo = false;
      digitalWrite(PINO_LED, LOW);
      lcd.backlight();
      Serial.println("Despertador desligado!");
      delay(500);
    }
  }

  // Atualizar LCD de forma otimizada
  atualizarLCD();

  delay(200);
}
