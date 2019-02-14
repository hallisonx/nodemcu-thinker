#include<ESP8266WiFi.h> 
#include <RCSwitch.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include<EasyDDNS.h>

//clima tempo
WiFiUDP ntpUDP;
int16_t utc = -3; //UTC -3:00 Brazil
uint32_t currentMillis = 0;
uint32_t previousMillis = 0;
NTPClient timeClient(ntpUDP, "a.st1.ntp.br", utc*3600, 60000);

//Modulo radio RF315
RCSwitch mySwitch = RCSwitch();

const char* ssid = "XXXXXX"; //your WiFi Name
const char* password = "YYYYYY";  //Your Wifi Password
String ip = "";
const int pino_13 = D7;

WiFiServer server(80);
void setup() {
  Serial.begin(115200);
  delay(10); 
  Serial.println();
  Serial.println();
  Serial.print("Conectando em ");
  Serial.println(ssid); 
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi conectado"); 
  server.begin();
  Serial.println("Servidor iniciado");
  Serial.print("Use esta URL para conectar: ");
  Serial.print("http://");
  IPAddress myIP = WiFi.localIP();
  Serial.print(myIP);
  Serial.println("/"); 
  ip =  String(myIP[0]) + "." + String(myIP[1]) + "." + String(myIP[2]) + "." + String(myIP[3]);

  // **** Radio Frequencia **** 
  // Transmitter is connected to Arduino Pin #0  (GPIO_0 = D3)
  mySwitch.enableTransmit(pino_13); //PIN 5,12,13 = D1,D6,D7 em nodeMcu
  delay(10);

//time
  timeClient.begin();
  timeClient.update();

//noip
  EasyDDNS.service("noip"); //"duckdns" / "noip" / "dyndns" / "dynu".
  EasyDDNS.client("seudominio.ddns.net","seu_usuario","sua_senha");
}

void loop() {
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  } 
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush(); 

  // envia o comando de radio 315mhz para o Thinker
  enviarComandosRF315(request);
  
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); 
  client.println("");
  client.println("");
  client.print("<b>Teste: 15 comandos gravados </b>"); 
  client.println("");  
  client.println(" <h1><a href='http://"+ip+"/CMD=01'>Comando 1</a> ");
  client.println(" <a href='http://"+ip+"/CMD=02'>Comando 2</a></h1> ");  
  client.println(" ");
  //Chama a verificacao de tempo
  client.println("<p>Time: " + checkOST() + "</p>");
  client.println(" ");
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");   

  //atualiza IP
  EasyDDNS.update(120000); //2m (60000*2)
}

void send (int msg) {
  mySwitch.send(msg, 24);
  delay(100);
}

void forceUpdate(void) {
  timeClient.forceUpdate();
}
 
String checkOST(void) {
  currentMillis = millis();//Tempo atual em ms
  //Lógica de verificação do tempo
  if (currentMillis - previousMillis > 1000) {
    previousMillis = currentMillis;    // Salva o tempo atual
    printf("Time Epoch: %d: ", timeClient.getEpochTime());
    Serial.println(timeClient.getFormattedTime());
    return timeClient.getFormattedTime();
  }
}

void enviarComandosRF315(String request){
  if (request.indexOf("/CMD=01") != -1)  {
    send(1394010);
  }
  if (request.indexOf("/CMD=02") != -1)  {
    send(1394020);
  }
  if (request.indexOf("/CMD=03") != -1)  {
    send(1394030);
  }
  if (request.indexOf("/CMD=04") != -1)  {
    send(1394040);
  }
  if (request.indexOf("/CMD=05") != -1)  {
    send(1394050);
  }
  if (request.indexOf("/CMD=06") != -1)  {
    send(1394060);
  }
  if (request.indexOf("/CMD=07") != -1)  {
    send(1394070);
  }
  if (request.indexOf("/CMD=08") != -1)  {
    send(1394080);
  }
  if (request.indexOf("/CMD=09") != -1)  {
    send(1394090);
  }
  if (request.indexOf("/CMD=10") != -1)  {
    send(1394100);
  }
  if (request.indexOf("/CMD=11") != -1)  {
    send(1394110);
  }
  if (request.indexOf("/CMD=12") != -1)  {
    send(1394120);
  }
  if (request.indexOf("/CMD=13") != -1)  {
    send(1394130);
  }
  if (request.indexOf("/CMD=14") != -1)  {
    send(1394140);
  }
  if (request.indexOf("/CMD=15") != -1)  {
    send(1394150);
  }
}
