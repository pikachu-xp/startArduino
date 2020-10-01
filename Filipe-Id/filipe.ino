#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <WiFiUdp.h>
#include <Wire.h>

unsigned int localPort = 2390; 

const char* ssid = "Nome Do Wifi";
const char* password = "Senha do Wifi";
const char* host = "Host";

IPAddress timeServerIP; 
const char* ntpServerName = "time.nist.gov";
const int NTP_PACKET_SIZE = 48;
byte packetBuffer[ NTP_PACKET_SIZE];

WiFiServer server(5000);
WiFiClientSecure client;
WiFiUDP udp;

unsigned long epoch;

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

unsigned long sendNTPpacket(IPAddress& address){
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  packetBuffer[0] = 0b11100011; 
  packetBuffer[1] = 0;   
  packetBuffer[2] = 6;     
  packetBuffer[3] = 0xEC;  
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  udp.beginPacket(address, 123); 
  udp.write(packetBuffer, NTP_PACKET_SIZE);
  udp.endPacket();
}

String getRequest(String quem){
  if ( !client.connect("mydream-ufpa-phi.herokuapp.com", 443) ) {
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
  if ( !client.connect("mydream-ufpa-phi.herokuapp.com", 443) ) {
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

/*void atualizarEstados(){
  String teste = getRequest("/sensor");
  ambientea = estados(teste.substring(774,783));
  tranca = estados(teste.substring(94,103));
  ventilador = estados(teste.substring(435,444));
  luz1 = estados(teste.substring(548,557));
  luz2 = estados(teste.substring(661,670));
  cortina = estados(teste.substring(323,332));
  janela = estados(teste.substring(208,217));
  alarme = estados(teste.substring(992,1001));
  senha = estados(teste.substring(885,889));  
  horaAcordar = (getRequest("/sensor/hora")).toInt();
  minutoAcordar = (getRequest("/sensor/minuto")).toInt();
}*/

void atualizarApi(){
  ambientea = estados(getRequest("/sensor/celular"));
  putRequest("/sensor/celular", voltaEstados(ambientea));
  putRequest("/sensor/e_tranca", voltaEstados(tranca));
  putRequest("/sensor/e_vent", voltaEstados(ventilador));
  putRequest("/sensor/e_lamp1", voltaEstados(luz1));
  putRequest("/sensor/e_lamp2", voltaEstados(luz2));
  putRequest("/sensor/e_cortina", voltaEstados(cortina));
  putRequest("/sensor/e_janela", voltaEstados(janela));
  putRequest("/sensor/alarme", voltaEstados(alarme));
}

void relogio(){
  WiFi.hostByName(ntpServerName, timeServerIP); 
  sendNTPpacket(timeServer); 
  
  delay(1000);
  if (Udp.parsePacket()) {
    Serial.println("packet received");
    Udp.read(packetBuffer, NTP_PACKET_SIZE); 
    
    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    unsigned long secsSince1900 = highWord << 16 | lowWord;
    Serial.print("Seconds since Jan 1 1900 = ");
    Serial.println(secsSince1900);

    Serial.print("Unix time = ");
    // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
    const unsigned long seventyYears = 2208988800UL;
    epoch = secsSince1900 - seventyYears;
    Serial.println(epoch);
  }
  delay(10000);
}

void setup (){
  conectar();
  relogio();
  putRequest("id", epoch);
}

//https://cityos.io/tutorial/1958/Use-multiplexer-with-Arduino multiplexador 1
//https://www.arduinoecia.com.br/2017/03/como-usar-pcf8574-expansor-de-portas-i2c.html teclado matricial
//http://www.nadielcomercio.com.br/blog/2015/09/01/ci-pcf8574-expansor-de-portas-io-8-bits-i2c-para-arduino/ multiplexador 2
//http://www.esp8266learning.com/wemos-mini-pcf8574.php 

void loop(){
 
}

