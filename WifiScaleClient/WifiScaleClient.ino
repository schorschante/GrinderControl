#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

// Set WiFi credentials
#define WIFI_SSID "TheOtherESP"
#define WIFI_PASS "flashmeifyoucan"
char incomingPacket[255];  // buffer for incoming packets

// UDP
WiFiUDP UDP; // @suppress("Abstract class cannot be instantiated")
IPAddress remote_IP(192,168,4,100);
#define UDP_PORT 4210

void setup() {

  // Setup IO
  pinMode(2, INPUT);

  // Setup serial port
  Serial.begin(115200);
  Serial.println();

  // Begin WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  WiFi.mode(WIFI_STA);

  // Connecting to WiFi...
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  // Loop continuously while WiFi is not connected
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(100);
    Serial.print(".");
  }

  // Connected to WiFi
  Serial.println();
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());

  // Begin UDP port
  UDP.begin(UDP_PORT);
  Serial.print("Opening UDP port ");
  Serial.println(UDP_PORT);

}

void loop() {

  // Read button

  // Send Packet
  UDP.beginPacket(remote_IP, UDP_PORT);
  UDP.write("test");
  UDP.endPacket();
  delay(100);

  int packetSize = UDP.parsePacket();
  	  if (packetSize)
  	  {
  	    // receive incoming UDP packets
  	    Serial.printf("Received %d bytes from %s, port %d\n", packetSize, UDP.remoteIP().toString().c_str(), UDP.remotePort());
  	    int len = UDP.read(incomingPacket, 255);
  	    if (len > 0)
  	    {
  	      incomingPacket[len] = 0;
  	    }
  	    Serial.printf("UDP packet contents: %s\n", incomingPacket);

  	  }

}
