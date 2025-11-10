#include <Arduino.h>
//Atividade 1
/*void setup()
{
  /*Serial.begin(9600);
  Serial.setTimeout(5000);

  String texto = Serial.readStringUntil('\n');
  texto.trim();
  Serial.println(texto);
  if (texto.endsWith("ON"))
  { 
  else
  {
    Serial.println("LED OFF");
  }
}
void loop()
{
}*/

/*atividade 2 


void setup() {
  Serial.begin(9600);

  String mensagem = "Temperatura = 27C";

  String numeros = "";
  for (char c : mensagem) {
    if (isDigit(c)) numeros += c;
  }

  int celsius = numeros.toInt();

  float fahrenheit = (celsius * 9.0 / 5.0) + 32.0;

  Serial.print("Celsius: ");
  Serial.print(celsius);
  Serial.print(" °C  ->  Fahrenheit: ");
  Serial.print(fahrenheit);
  Serial.println(" °F");
}

void loop() {

}
*/

/*Atividade 3
void setup()
{
  Serial.begin(9600);
  
  String frase = "ESTAMOS APRENDENDO ARDUINO NA AULA";
  frase.trim();
  String palavra_chave = "ARDUINO";
  if (frase.){
  Serial.println ("TEM");
  }
  else{
    Serial.println ("NÃO TEM");

}
}
void loop()
  {
    }*/
//Atividade 4
    /*void setup()
{
  Serial.begin(9600);
  String frase = "sEnAi eSp32 ArDuInO";
  frase.toUpperCase();
  Serial.println (frase);

  frase.toLowerCase();
  Serial.println (frase);
}
void loop()
  {
    }*/