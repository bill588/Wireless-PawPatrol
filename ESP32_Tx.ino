#include <WiFi.h>
#include <WiFiUdp.h>
#include <RCSwitch.h>

const char* ssid = "Link_Router";
const char* password = "YouJustCantKnow11364";
const int udpPort = 5005;

WiFiUDP udp;
RCSwitch mySwitch = RCSwitch();

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  Serial.print("ESP32 IP address: ");
  Serial.println(WiFi.localIP());

  // Start UDP listener
  udp.begin(udpPort);
  Serial.printf("Listening for UDP packets on port %d\n", udpPort);

  // Setup 433 MHz transmitter
  mySwitch.enableTransmit(12);   // GPIO 12 connected to TX module DATA
  mySwitch.setPulseLength(350);
  mySwitch.setRepeatTransmit(10);
}

void loop() {
  int packetSize = udp.parsePacket();
  if (packetSize) {
    char buf[255];
    int len = udp.read(buf, 255);
    if (len > 0) buf[len] = '\0';

    String msg = String(buf);
    Serial.println("Received: " + msg);

    if (msg.startsWith("DETECTED:")) {
      if (msg.indexOf("dog") > 0) {
        Serial.println("Sending RF code for dog");
        mySwitch.send(11111, 24);
      }
      else if (msg.indexOf("cat") > 0) {
        Serial.println("Sending RF code for cat");
        mySwitch.send(22222, 24);
      }
      else {
        Serial.println("Sending RF code default");
        mySwitch.send(12345, 24);
      }
    }
  }
}
