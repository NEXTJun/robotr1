#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>

#define Serial_Speed      115200

#define Project_SSID      "NEXTJun_Robot_R1"
#define Project_PASSWORD  "0916163689"
#define Project_HOST      "www.nextjun_robot.com"

#define UsePassword_Flag  false

const char* ssid = Project_SSID;
const char* password = Project_PASSWORD;
const char* dns_host = Project_HOST;

const byte DNS_PORT = 53;
const byte TCP_PORT = 80;
const byte Sockets_PORT = 81;

IPAddress apIP(192, 168, 100, 1);       // IP, gateway
IPAddress apNetmask(255, 255, 255, 0);  // netmask
ESP8266WebServer webServer(TCP_PORT);
DNSServer dnsServer;
WebSocketsServer webSocket = WebSocketsServer(Sockets_PORT);

String page = "";
const byte LEDPin = 2;

void setup() {
  page = "<h1>Simple NodeMCU Web Server</h1><p><a href=\"LEDOn\"><button>ON</button></a>&nbsp;<a href=\"LEDOff\"><button>OFF</button></a></p>";
  ledConfig();
  serialConfig();
  apConfig();
  dnsConfigBegin();
  apHandle();
  apBegin();
}

void loop() {
  dnsServer.processNextRequest();
  webSocket.loop();
  webServer.handleClient();
  if (Serial.available() > 0){
    char c[] = {(char)Serial.read()};
    webSocket.broadcastTXT(c, sizeof(c));
  }
}

void ledConfig(){
 pinMode(LEDPin, OUTPUT);
 digitalWrite(LEDPin, LOW);

 delay(500);
}

void serialConfig(){
  Serial.begin(Serial_Speed);
}

void apConfig(){
  Serial.println();

  Serial.print("ESP8266 mode WIFI_AP ... ");
  Serial.println(WiFi.mode(WIFI_AP) ? "Ready" : "Failed!");

  Serial.print("Setting soft-AP IP configuration ... ");
  Serial.println(WiFi.softAPConfig(apIP, apIP, apNetmask) ? "Ready" : "Failed!");

  Serial.print("Setting soft-AP Server configuration ... ");
  if(UsePassword_Flag){
    Serial.println(WiFi.softAP(ssid, password) ? "Ready" : "Failed!");
  } else {
    Serial.println(WiFi.softAP(ssid) ? "Ready" : "Failed!");
  }
  
  Serial.print("Soft-AP IP address = ");
  Serial.println(WiFi.softAPIP());
}

void apHandle(){
  webServer.on("/",handle_Root);
  webServer.on("/LEDOn",handle_LED_On);
  webServer.on("/LEDOff",handle_LED_Off);
  webServer.onNotFound(handle_NotFound);
}

void apBegin(){
  webServer.begin();
  Serial.println("HTTP server started");
}

void dnsConfigBegin(){
  Serial.println();
  
  Serial.print("Setting mDNS Server configuration ... ");
  dnsServer.setTTL(300);
  dnsServer.setErrorReplyCode(DNSReplyCode::ServerFailure);  
  Serial.println("Ready");
    
  Serial.print("Setting mDNS Server configuration ... ");
  while (!dnsServer.start(DNS_PORT, dns_host, apIP)) {
    Serial.println("Failed!");
    Serial.print("Wait 1 second try again ...");
    delay(1000);
  }
  Serial.println("Ready");
  Serial.println("DNS server started");
}

void socketConfigBegin(){
 webSocket.begin();
 webSocket.onEvent(webSocketEvent);
 Serial.println("Socket server started");
}

void handle_Root(){
  webServer.send(200, "text/html", page);
}

void handle_LED_On(){
  webServer.send(200, "text/html", page);
    digitalWrite(LEDPin, HIGH);
    delay(1000);
}

void handle_LED_Off(){
  webServer.send(200, "text/html", page);
    digitalWrite(LEDPin, LOW);
    delay(1000);
}

void handle_NotFound(){
  webServer.send(404, "text/plain", "Not found");
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length){
  if (type == WStype_TEXT){
   for(int i = 0; i < length; i++) Serial.print((char) payload[i]);
   Serial.println();
  }
}
/*

snprintf ( html, 1000,

"<html>\
  <head>\
    <meta http-equiv='refresh' content='10'/>\
    <title>ESP8266 WiFi Network</title>\
    <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; font-size: 1.5em; Color: #000000; }\
      h1 { Color: #AA0000; }\
    </style>\
  </head>\
  <body>\
    <h1>ESP8266 Wi-Fi Access Point and Web Server Demo</h1>\
    <p>Uptime: %02d:%02d:%02d</p>\
    <p>Brightness: %d%</p>\
    <p>%s<p>\
    <p>This page refreshes every 10 seconds. Click <a href=\"javascript:window.location.reload();\">here</a> to refresh the page now.</p>\
  </body>\
</html>",

    hr, min % 60, sec % 60,
    brightness,
    ledText
  );

  */
