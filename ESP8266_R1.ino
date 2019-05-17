#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>
#include <FS.h>

#define Serial_Speed      115200

#define Project_SSID      "Robot_R1"
#define Project_PASSWORD  "0916163689"
#define Project_HOST      "www.robot.com"

#define UsePassword_Flag  false

# define LED_R 14
# define LED_G 12
# define LED_B 13

# define LED_SIG 16

#define jsonBufferSize 100

const char* ssid = Project_SSID;
const char* password = Project_PASSWORD;
const char* dns_host = Project_HOST;

const byte DNS_PORT = 53;
const byte TCP_PORT = 80;
const byte Sockets_PORT = 81;

IPAddress apIP(192, 168, 0, 1);       // IP, gateway
IPAddress apNetmask(255, 255, 255, 0);  // netmask
ESP8266WebServer webServer(TCP_PORT);
DNSServer dnsServer;
WebSocketsServer webSocket = WebSocketsServer(Sockets_PORT);

DynamicJsonDocument jsonBuffer(jsonBufferSize);

void setup() {      
  ledConfig();
  serialConfig();
  spiffsConfig();
  apConfig();
  dnsConfigBegin();
  socketConfigBegin();
  apHandle();
  apBegin();
}

void loop() {
  dnsServer.processNextRequest();
  webSocket.loop();
  webServer.handleClient();
  if (Serial.available() > 0) {
    char c[] = {(char)Serial.read()};
    webSocket.broadcastTXT(c, sizeof(c));
  }
}

void ledConfig() {
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);
  pinMode(LED_SIG, OUTPUT);

  digitalWrite(LED_R, HIGH);
  digitalWrite(LED_G, HIGH);
  digitalWrite(LED_B, HIGH);

  digitalWrite(LED_SIG, HIGH);
  delay(2000);
  digitalWrite(LED_SIG, LOW);
}

void serialConfig() {
  Serial.begin(Serial_Speed);
}

void spiffsConfig() {
  Serial.print("Setting SPIFFS configuration ... ");
  Serial.println(SPIFFS.begin() ? "Ready" : "Failed!");  
}

void apConfig() {
  Serial.println();

  Serial.print("ESP8266 mode WIFI_AP ... ");
  Serial.println(WiFi.mode(WIFI_AP) ? "Ready" : "Failed!");

  Serial.print("Setting soft-AP IP configuration ... ");
  Serial.println(WiFi.softAPConfig(apIP, apIP, apNetmask) ? "Ready" : "Failed!");

  Serial.print("Setting soft-AP Server configuration ... ");
  if(UsePassword_Flag) {
    Serial.println(WiFi.softAP(ssid, password) ? "Ready" : "Failed!");
  } else {
    Serial.println(WiFi.softAP(ssid) ? "Ready" : "Failed!");
  }
  
  Serial.print("Soft-AP IP address = ");
  Serial.println(WiFi.softAPIP());
}

void apHandle() {
  webServer.onNotFound(webHandle);
}

void apBegin() {
  webServer.begin();
  Serial.println("HTTP server started");
}

void dnsConfigBegin() {
  Serial.println();
  
  Serial.print("Setting DNS Server configuration ... ");
  dnsServer.setTTL(300);
  dnsServer.setErrorReplyCode(DNSReplyCode::ServerFailure);  
  Serial.println("Ready");
    
  Serial.print("Setting DNS Server start ... ");
  while (!dnsServer.start(DNS_PORT, dns_host, apIP)) {
    Serial.println("Failed!");
    Serial.print("Wait 1 second try again ...");
    delay(1000);
  }
  Serial.println("Ready");
  Serial.println("DNS server started");
}

void socketConfigBegin() {
 webSocket.begin();
 webSocket.onEvent(webSocketEvent);
 Serial.println("Socket server started");
}

bool handleFileRead(String path){
  if (path.endsWith("/")) {
    path += "index.html";
  } 
  String contentType = getContentType(path);
  String pathWithGz = path + ".gz";
  
  if (SPIFFS.exists(path) || SPIFFS.exists(pathWithGz)){
    if(SPIFFS.exists(pathWithGz)) {
      path += ".gz";
    }
    
    File file = SPIFFS.open(path, "r");
    webServer.streamFile(file, contentType);
    file.close(); 
    return true;
  }
  return false;
}

void webHandle() {
    if (!handleFileRead(webServer.uri())) {
      webServer.send(404, "text/plain", "FileNotFound");
    }
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  switch(type) {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", num);
      break;

    case WStype_CONNECTED: {
      IPAddress ip = webSocket.remoteIP(num);
      Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);      
      webSocket.sendTXT(num, "esp8266 Connected");
      }  // send message to client
      break;

    case WStype_TEXT:
      Serial.printf("[%u] get Text: %s\n", num, payload);      
      handleSetting(payload);
      break;
  }
}

void handleSetting(uint8_t * payload){
  DeserializationError error = deserializeJson(jsonBuffer, payload);
  if (error) {
    Serial.printf("analysis JSON failed!"); 
  } else {  
    int rgb_r = jsonBuffer["r"];
    int rgb_g = jsonBuffer["g"];
    int rgb_b = jsonBuffer["b"];
    bool sig_led = jsonBuffer["signal_led"];
  
    analogWrite(LED_R, map(rgb_r, 0, 255, 1023, 0));
    analogWrite(LED_G, map(rgb_g, 0, 255, 1023, 0));
    analogWrite(LED_B, map(rgb_b, 0, 255, 1023, 0)); 
    digitalWrite(LED_SIG, sig_led);
  }  
}

String getContentType(String filename){
  if(webServer.hasArg("download")) return "application/octet-stream";
  else if(filename.endsWith(".htm")) return "text/html";
  else if(filename.endsWith(".html")) return "text/html";
  else if(filename.endsWith(".css")) return "text/css";
  else if(filename.endsWith(".js")) return "application/javascript";
  else if(filename.endsWith(".png")) return "image/png";
  else if(filename.endsWith(".gif")) return "image/gif";
  else if(filename.endsWith(".jpg")) return "image/jpeg";
  else if(filename.endsWith(".ico")) return "image/x-icon";
  else if(filename.endsWith(".xml")) return "text/xml";
  else if(filename.endsWith(".pdf")) return "application/x-pdf";
  else if(filename.endsWith(".zip")) return "application/x-zip";
  else if(filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}
