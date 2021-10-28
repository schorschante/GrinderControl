#include "Arduino.h"
#include "ESP8266WiFi.h"
//The setup function is called once at startup of the sketch
const char* ssid = "Martin Router King";
const char* password = "AndelUndIch1994";


WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  delay(10);

  // Mit Wifi verbinden
  Serial.print("Verbinden mit: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi verbunden");

  // Start des Servers
  server.begin();
  Serial.println("Server gestartet");

  // Print the IP address
  Serial.print("Diese URL zum Verbinden aufrufen: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
}

void loop() {
  // Prüfen, ob sich ein Client verbunden hat
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Warten auf Daten vom Client
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }

  // Erste Zeile des Requests lesen
  String request = client.readStringUntil('\r');
  request.replace("GET /", "");
  request.replace(" HTTP/1.1", "");
  request.replace("TIME=", "");
  Serial.println(request.toInt());
  client.flush();

  // Match the request

  // Anfrage zurücksenden
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");

  client.print("Die LED ist: ");


  client.println("<br><br>");
  client.println("<a href=\"/TIME=176687687\"\"><button>An </button></a>");
  client.println("<a href=\"/TIME=121414\"\"><button>Aus </button></a><br />");
  client.println("</html>");

  delay(1);
  Serial.println("Client trennen");
  Serial.println("");
}
