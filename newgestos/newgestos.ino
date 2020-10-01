#include "Adafruit_APDS9960.h" //INCLUSÃO DE BIBLIOTECA
Adafruit_APDS9960 apds; //OBJETO DO TIPO Adafruit_APDS9960 (I2C)
 
void setup() {
  Serial.begin(9600); //INICIALIZA A SERIAL
  
  if(!apds.begin()){ //SE O SENSOR NÃO FOR INICIALIZADO, FAZ
    Serial.println("Falha ao inicializar o dispositivo. Verifique as conexões!"); //IMPRIME O TEXTO NO MONITOR SERIAL
  }//SENÃO, FAZ
  else Serial.println("Dispositivo inicializado!"); //IMPRIME O TEXTO NO MONITOR SERIAL
 
  //O MODO DETECÇÃO DE GESTO SERÁ HABILITADO QUANDO O SENSOR DETECTAR ALGO PRÓXIMO DO MÓDULO (APROXIME SEU DEDO E AFASTE)
  apds.enableProximity(true);
  apds.enableGesture(true);
}
 
void loop() {
    uint8_t gesture = apds.readGesture(); //FAZ A LEITURA DA DIREÇÃO DO GESTO
    if(gesture == APDS9960_DOWN) Serial.println("↓"); //SE DETECTAR GESTO (DE CIMA PARA BAIXO), IMPRIME O SÍMBOLO NO MONITOR SERIAL
    if(gesture == APDS9960_UP) Serial.println("↑"); //SE DETECTAR GESTO (DE BAIXO PARA CIMA), IMPRIME O SÍMBOLO NO MONITOR SERIAL
    if(gesture == APDS9960_LEFT) Serial.println("←"); //SE DETECTAR GESTO (DA DIREITA PARA A ESQUERDA), IMPRIME O SÍMBOLO NO MONITOR SERIAL
    if(gesture == APDS9960_RIGHT) Serial.println("→"); //SE DETECTAR GESTO (DA ESQUERDA PARA A DIREITA), IMPRIME O SÍMBOLO NO MONITOR SERIAL
}
