#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

// Set AP credentials
#define AP_SSID "TheOtherESP"
#define AP_PASS "flashmeifyoucan"

char incomingPacket[255];  // buffer for incoming packets
char  replyPacket[] = "Hi there! Got the message :-)";  // a reply string to send back


// UDP
WiFiUDP UDP; // @suppress("Abstract class cannot be instantiated")
IPAddress local_IP(192,168,4,100);
IPAddress gateway(192,168,4,100);
IPAddress subnet(255,255,255,0);
#define UDP_PORT 4210

// UDP Buffer
char packetBuffer[UDP_TX_PACKET_MAX_SIZE];

void setup() {

  // Setup LED pin
  pinMode(2, OUTPUT);

  // Setup serial port
  Serial.begin(57600);
  Serial.println();

  // Begin Access Point
  Serial.println("Starting access point...");
  WiFi.softAPConfig(local_IP, gateway, subnet);
  WiFi.softAP(AP_SSID, AP_PASS);
  Serial.println(WiFi.localIP());

  // Begin listening to UDP port
  UDP.begin(UDP_PORT);
  Serial.print("Listening on UDP port ");
  Serial.println(UDP_PORT);

}

void loop() {

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

	    // send back a reply, to the IP address and port we got the packet from
	    UDP.beginPacket(UDP.remoteIP(), UDP.remotePort());
	    UDP.write(replyPacket);
	    UDP.endPacket();
	  }
}
