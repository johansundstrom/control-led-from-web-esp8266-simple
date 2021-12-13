#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include "index.h"
#include <ArduinoJson.h>  //https://arduinojson.org/
#include "includes.h"
//https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WebServer
//https://links2004.github.io/Arduino/d3/d58/class_e_s_p8266_web_server.html#a84e92e16e4c420065c474eb54bb26153
ESP8266WebServer www_server(80); //create an instance


extern const char *ssid;
extern const char *password;

const int LED = LED_BUILTIN;
String server_state = "";



void connectToWiFi() {
  Serial.println("Connecting to the WiFi");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("Waiting for connection");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}



void setupServer() {
  www_server.begin();             //Start
  Serial.println("HTTP server started");
  www_server.on("/", htmlIndex);  //Client request handlerFunction
  www_server.on("/api", led);     //Client request handlerFunction
}



//hantera användarklick (från setupServer)
void led() {
  const char* state;
  //server_state = www_server.arg("browser_state"); //Getting information about request arguments
  
  //Hantera inkommande request JSON
  if (www_server.hasArg("plain")== true) {
    String jsondata = www_server.arg("plain");
    //JSON ==> Object
    StaticJsonDocument<64> req_json;
    DeserializationError error = deserializeJson(req_json, jsondata);
    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.f_str());
      return;
    }
    //Plocka ut data ur JSON
    state = req_json["state"];
    
    StaticJsonDocument<100> res_json;
    res_json["state"] = state;
    char buffer[100];
    serializeJson(res_json, buffer);
    //buffer = JSON
    www_server.send(200, "application/json", buffer);
  }
  //Serial.print("state: ");
  //Serial.println(state);
  
  //if (state == "ON") {
  if (strlen(state) == 2) {
    digitalWrite(LED, HIGH);
    Serial.println("LED ON");
  } else {
    digitalWrite(LED, LOW);
    Serial.println("LED OFF");
  }
}



//sänd html-sida och HTTP200
void htmlIndex() {
  String res_page = FPSTR(MAIN_page);
  www_server.send(200, "text/html", res_page);
}



void setup() {
  pinMode(LED, OUTPUT); //builtin
  Serial.begin(115200);
  delay(1000);
  connectToWiFi();
  setupServer();
  delay(4000);
}



void loop() {
  www_server.handleClient();      //Listen for HTTP requests from clients  
}
