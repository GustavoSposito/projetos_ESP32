#include <Arduino.h>
#include <Bounce2.h>

/*const int pinoBotao = 23;   
const int pinoLED = 22;    
int cont = 0;
int estadoBotao;           
int ultimoEstadoBotao = HIGH; 
int estadoLED = LOW;       

void setup() {
  Serial.begin(9600);
  pinMode(pinoLED, OUTPUT);       
  pinMode(pinoBotao, INPUT_PULLUP); 
}

void loop() {
  estadoBotao = digitalRead(pinoBotao);

  if (estadoBotao != ultimoEstadoBotao) {
    delay(50); 

    if (estadoBotao == HIGH ) { 
      estadoLED = !estadoLED; 
      cont++;
      digitalWrite(pinoLED, estadoLED);

    }
  }

  ultimoEstadoBotao = estadoBotao;
  Serial.println(cont);
}*/

/*const int pinoBotaoMais = 23;   
const int pinoBotaoMenos = 21;  
const int pinoLED = 22;         


Bounce botaoMais = Bounce();
Bounce botaoMenos = Bounce();


int brilho = 0;  

void setup() {
  Serial.begin(9600);


  pinMode(pinoLED, OUTPUT);
  pinMode(pinoBotaoMais, INPUT_PULLUP);
  pinMode(pinoBotaoMenos, INPUT_PULLUP);

  botaoMais.attach(pinoBotaoMais);
  botaoMais.interval(50);

  botaoMenos.attach(pinoBotaoMenos);
  botaoMenos.interval(50);
}

void loop() {
  botaoMais.update();
  botaoMenos.update();

  if (botaoMais.fell()) {
    brilho += 10;             
    if (brilho > 255) brilho = 255;  
    analogWrite(pinoLED, brilho);
    Serial.print("Brilho aumentado para: ");
    Serial.println(brilho);
  }

  // 🟥 Se o botão "diminuir" foi pressionado
  if (botaoMenos.fell()) {
    brilho -= 10;             // diminui 10%
    if (brilho < 0) brilho = 0;  // limite mínimo
    analogWrite(pinoLED, brilho);
    Serial.print("Brilho diminuído para: ");
    Serial.println(brilho);
  }
}*/
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);

const int pinoBotaoMais = 5;
const int pinoBotaoMenos = 23;
const int pinoLED = 2;

Bounce botaoMais = Bounce();
Bounce botaoMenos = Bounce();

int brilho = 0;

void setup()
{
  Serial.begin(9600);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Controle de LED");

  pinMode(pinoLED, OUTPUT);
  pinMode(pinoBotaoMais, INPUT_PULLUP);
  pinMode(pinoBotaoMenos, INPUT_PULLUP);

  botaoMais.attach(pinoBotaoMais);
  botaoMais.interval(50);

  botaoMenos.attach(pinoBotaoMenos);
  botaoMenos.interval(50);
}

void loop()
{

  botaoMais.update();
  botaoMenos.update();

  if (botaoMais.fell())
  {
    brilho += 10;
    if (brilho > 255)
      brilho = 255;
    analogWrite(pinoLED, brilho);
    lcd.setCursor(0,1);
    lcd.print(String(brilho) + " % ");
    Serial.print("Brilho aumentado para: ");
    Serial.println(brilho);
  }

  if (botaoMenos.fell())
  {
    brilho -= 10;
    if (brilho < 0)
      brilho = 0;
    analogWrite(pinoLED, brilho);
    lcd.setCursor(0,1);
    lcd.print(String(brilho) + " % ");
    Serial.print("Brilho diminuído para: ");
    Serial.println(brilho);
    
  }
}

void atualizaLCD() {
  
  float porcentagem = (brilho / 255.0) * 100.0; 

  lcd.setCursor(0, 1);
  lcd.print("Brilho: ");
  lcd.print((int)porcentagem);
  lcd.print("%    ");
   atualizaLCD();

}
