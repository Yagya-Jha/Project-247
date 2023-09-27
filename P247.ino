#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
const char ssid[] = "Bipin";
const char password[] = "8ispalak@1234";

byte r_pin = 18;
byte g_pin = 22;
byte b_pin = 23;

String generate_html(){
  
  String html = "<!DOCTYPE html>\n";
         html += "<html lang=\"en\">\n";
         html += "<head>\n";
         html += "<meta charset=\"UTF-8\">\n";
         html += "<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\n";
         html += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
         html += "<title>ESP32 RGB CONTROLLER</title>\n";
         html += "</head>\n";
         html += "<body bgcolor=black>\n";
         html += "<div style=\"text-align: center;\">\n";
         html += "<h1 style = 'color: white'>COLOR GENERATOR</h1>\n";
         html += "</div>\n";
         html += "<div style=\"margin: auto; width: 80%;\">\n";
         html += "<a href=\"/red\"><button style = 'background-color: red; color: white'>Red</button></a>\n";
         html += "<a href=\"/green\"><button style = 'background-color: green; color: white'>Green</button></a>\n";
         html += "<a href=\"/blue\"><button style = 'background-color: blue; color: white'>Blue</button></a>\n";
         html += "<a href=\"/yellow\"><button style = 'background-color: yellow; color: white'>Yellow</button></a>\n";
         html += "<a href=\"/cyan\"><button style = 'background-color: cyan; color: white'>Cyan</button></a>\n";
         html += "<a href=\"/magenta\"><button style = 'background-color: magenta; color: white'>Magenta</button></a>\n";
         html += "<a href=\"/orange\"><button style = 'background-color: orange; color: white'>Orange</button></a>\n";
         html += "</div>\n";    
         html += "</body>\n";
         html += "</html>\n";

  return html;
}
String button = generate_html();

//  defining channels
#define r_channel 0
#define g_channel 1
#define b_channel 2

//  defining frequency and reolution
#define frequency 5000
#define resolution 8

//  starting a server at port 80
WebServer server(80);

void handleRoot(){
  server.send(200, "text/html", button);
}

void handleNotFound() {
  
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
 
}

void setup(){

  //  setting frequency and resolution at channels
  ledcSetup(r_channel , frequency , resolution);
  ledcSetup(g_channel , frequency , resolution);
  ledcSetup(b_channel , frequency , resolution);

  //  attaching GPIOs
  ledcAttachPin(r_pin , r_channel);
  ledcAttachPin(g_pin , g_channel);
  ledcAttachPin(b_pin , b_channel);

  //   starting serial communication
  Serial.begin(115200);

  //  connecting with the WiFi and printing IP Address
  Serial.print("Connecting with : ");
  Serial.println(ssid);
  WiFi.begin(ssid , password);
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.println("Connected with the Network !");
  Serial.print("IP address assgined by the access point : ");
  Serial.println(WiFi.localIP());

  if(MDNS.begin("esp32")){
    Serial.println("MDNS responder Started");
  }
  server.on("/", handleRoot);

  server.on("/red", [](){
    server.send(200, "text/html", button);
    color_generator(255,0,0);
  });
  server.on("/green", [](){
    server.send(200, "text/html", button);
    color_generator(0,255,0);
  });
  server.on("/blue", [](){
    server.send(200, "text/html", button);
    color_generator(0,0,255);
  });
  server.on("/yellow", [](){
    server.send(200, "text/html", button);
    color_generator(255,255,0);
  });
  server.on("/cyan", [](){
    server.send(200, "text/html", button);
    color_generator(0,255,255);
  });
  server.on("/magenta", [](){
    server.send(200, "text/html", button);
    color_generator(255,0,255);
  });
  server.on("/orange", [](){
    server.send(200, "text/html", button);
    color_generator(255,100,0);
  });
  
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP Server started");
  color_generator(100,100,100);
}

void loop(void){
  server.handleClient();
}

void color_generator(byte r , byte g , byte b){
  ledcWrite(r_channel , r);
  ledcWrite(g_channel , g);
  ledcWrite(b_channel , b);
}