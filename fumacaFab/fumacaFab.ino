#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <Wire.h>

unsigned int localPort = 2390; 

const char* ssid = "Nome Do Wifi";
const char* password = "Senha do Wifi";
const char* host = "Host";

const int AOUTpin = 0;
const int DOUTpin = 8;
const int ledPin = 13;

int limit;
int value;

WiFiServer server(5000);
WiFiClientSecure client;

void conectar(){
  Serial.begin(115200);
  Serial.println();
  Serial.print("connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  udp.begin(localPort);

  Serial.print("connecting to ");
  Serial.println(host);
  server.begin();
  if (!client.connect(host, 443)) {
    Serial.println("connection failed");
    return;
  }
}

String getRequest(String quem){
  if ( !client.connect(host, 443) ) {
    online = false;
    Serial.println("conexao falhou");
    return "connection failed";
  }

  online = true;
  
  client.print(String("GET ") + quem + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: BuildFailureDetectorESP8266\r\n" +
               "Connection: close\r\n\r\n");
 
  while (client.connected()) {
    
    String line = client.readStringUntil('\n');
    //Serial.println(line);
    if (line == "\r") {
      break;
    }
  }
  String line = client.readStringUntil('\n');
  Serial.println(line);
  Serial.println("saiu do while");
  return line.substring(1, line.length() - 1);  
}

void putRequest(String quem, String estado){
  if ( !client.connect(host, 443) ) {
    Serial.println("conexao falhou");
    online = false;
    return;
  }

  online = true;

  String data = "{\"estado\":\"" + estado + "\"}";

  client.print(String("PUT ") + quem + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Content-Type: application/json\r\n" +
                 "Content-Length: " + data.length() + "\r\n" +
                 "\r\n" + 
                 data + "\n");
}

bool estados(String result){
  if(result == "aaaligado"){
    return true;
  } else{
    return false;
  }
}

String voltaEstados(bool result){
  if(result == true){
    return "aaaligado";
  } else{
    return "desligado";
  }
}

void setup (){
  conectar();
  pinMode(DOUTpin, INPUT);
  pinMode(ledPin, OUTPUT);
}


void loop(){
  value= analogRead(AOUTpin);
  limit= digitalRead(DOUTpin);
  Serial.print("CO value: ");
  Serial.println(value);
  Serial.print("Limit: ");
  Serial.println(limit);
  delay(100);
  if (limit == HIGH){
    putRequest("caminho", "estado"); //muita fumaça no ambiente
  }else{
    putRequest("caminho", "estado"); //pouca fumaça no ambiente
  }
}


/*
References
http://osoyoo.com/driver/mq7.txt
https://www.embarcados.com.br/esp8266-com-arduino/
http://osoyoo.com/2016/06/15/mq-7-co-gas/
http://osoyoo.com/wp-content/uploads/2016/06/MQ-7_bb.jpg
https://www.robocore.net/loja/produtos/adaptador-para-esp8266.html
https://www.filipeflop.com/blog/esp8266-arduino-tutorial/
*/

