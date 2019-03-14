#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>

#define Serial_Speed      115200

#define Project_SSID      "NEXTJun_Robot_R1"
#define Project_PASSWORD  "0916163689"
#define Project_HOST      "www.nextjun_robot.com"

#define UsePassword_Flag  false

# define LED_R D7
# define LED_G D6
# define LED_B D5

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

String page = "\
  <html>\
    <head>\
      <script>\
        var connection = new WebSocket('ws://'+location.hostname+':81/', ['arduino']);\
        connection.onopen = function() {\
          connection.send('Connect ' + new Date());\
        };\
        connection.onerror = function(error) {\
          console.log('WebSocket Error ', error);\
        };\
        connection.onmessage = function(e) {\
          console.log('Server: ', e.data);\
        };\
        function sendRGB() {\
          var r = parseInt(document.getElementById('r').value).toString(16);\
          var g = parseInt(document.getElementById('g').value).toString(16);\
          var b = parseInt(document.getElementById('b').value).toString(16);\
          if(r.length < 2) { r = '0' + r; }\
          if(g.length < 2) { g = '0' + g; }\
          if(b.length < 2) { b = '0' + b; }\
          var rgb = '#'+r+g+b;\
          console.log('RGB: ' + rgb);\
          connection.send(rgb);\
        }\
      </script>\
    </head>\
    <body>\
      LED Control:<br/><br/>\
      R: <input id=\"r\" type=\"range\" min=\"0\" max=\"255\" step=\"1\" oninput=\"sendRGB();\" /><br/>\
      G: <input id=\"g\" type=\"range\" min=\"0\" max=\"255\" step=\"1\" oninput=\"sendRGB();\" /><br/>\
      B: <input id=\"b\" type=\"range\" min=\"0\" max=\"255\" step=\"1\" oninput=\"sendRGB();\" /><br/>\
    </body>\
  </html>\
";

void setup() {      
  ledConfig();
  serialConfig();
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

  digitalWrite(LED_R, LOW);
  digitalWrite(LED_G, LOW);
  digitalWrite(LED_B, LOW);
}

void serialConfig() {
  Serial.begin(Serial_Speed);
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
  webServer.on("/", handle_Root);
  webServer.onNotFound(handle_NotFound);
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
    
  Serial.print("Setting DNS Server configuration ... ");
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

void handle_Root() {
  webServer.send(200, "text/html", page);
}

void handle_NotFound() {
  webServer.send(404, "text/plain", "Not found");
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  switch(type) {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", num);
      break;

    case WStype_CONNECTED:{
      IPAddress ip = webSocket.remoteIP(num);
      Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);      
      webSocket.sendTXT(num, "Connected");}  // send message to client
      break;

    case WStype_TEXT:
      Serial.printf("[%u] get Text: %s\n", num, payload);
      if(payload[0] == '#') {
        // we get RGB data
        // decode rgb data
        uint32_t rgb = (uint32_t) strtol((const char *) &payload[1], NULL, 16);
        analogWrite(LED_R, map(((rgb >> 16) & 0xFF), 0, 255, 0, 1023));
        analogWrite(LED_G, map(((rgb >> 8) & 0xFF), 0, 255, 0, 1023));
        analogWrite(LED_B, map(((rgb >> 0) & 0xFF), 0, 255, 0, 1023));
      }
      break;
  }
}
