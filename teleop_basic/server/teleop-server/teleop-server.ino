#include <WiFi.h>

const char* ssid = "minibotv3-proto";
// password needs to be 8 chars minimum due to wifi standards
const char* pass = "hjklhjkl";

WiFiUDP udp;

void setup()
{
  Serial.begin(115200);

  WiFi.disconnect(true);
  WiFi.softAP(ssid, pass);

  Serial.println(udp.begin(1234));

  IPAddress IP = WiFi.softAPIP();
  Serial.print("IP address: ");
  Serial.println(IP);

}

char buffer[127]; //buffer to hold incoming packet
int left = 0;
int right = 0;
int timeout = millis();
bool showWarning = false;

void loop() {
  int packetSize = udp.parsePacket();

  if (packetSize) {
    timeout = millis();
    // read the packet into packetBufffer
    int len = udp.read(buffer, 127);
    if (len > 0) {
      buffer[len] = 0;
    }
    int power = (int)buffer[len-1];
    left = power >> 2;
    right = power & 3;
    Serial.printf("left: %d\tright: %d\n", left, right);
  }
  if (millis() - timeout > 500) {
    if (showWarning == false) {
      showWarning = true;
      Serial.println("disconnected");
    }
    
    left = 0;
    right = 0;
  } else {
    showWarning = false;
  }
}