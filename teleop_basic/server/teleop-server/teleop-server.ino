#include <WiFi.h>
#define MAXSPEED 100 // 0-255
#define INERTIA 10 // 0-255

const char* ssid = "minibotv3-proto";
// password needs to be 8 chars minimum due to wifi standards
const char* pass = "hjklhjkl";

WiFiUDP udp;

// comms
char buffer[127]; //buffer to hold incoming packet

// control 
int leftComm = 0;
int rightComm = 0;
int leftSpeed = 0;
int rightSpeed = 0;

// motor pins
const int leftFwd = 7;
const int leftBwd = 8;
const int rightFwd = 9;
const int rightBwd = 10;

// for timeout on disconnect 
int timeout = millis();
bool showWarning = false;

void setup() {
  // serial
  Serial.begin(115200);

  // wifi and comms
  WiFi.disconnect(true);
  WiFi.softAP(ssid, pass);

  Serial.println(udp.begin(1234));

  IPAddress IP = WiFi.softAPIP();
  Serial.print("IP address: ");
  Serial.println(IP);

  // motors
  pinMode(leftFwd, OUTPUT);
  pinMode(leftBwd, OUTPUT);
  pinMode(rightFwd, OUTPUT);
  pinMode(rightBwd, OUTPUT);
}

void loop() {
  // parse packet
  int packetSize = udp.parsePacket();
  if (packetSize) {
    timeout = millis();
    // read the packet into packetBufffer
    int len = udp.read(buffer, 127);
    if (len > 0) {
      buffer[len] = 0;
    }
    int power = (int)buffer[len-1];
    leftComm = power >> 2;
    rightComm = power & 3;

    leftComm = (-1 * (leftComm >> 1)) * (leftComm & 1);
    rightComm = (-1 * (rightComm >> 1)) * (rightComm & 1);

    Serial.printf("comms\tleft: %d\tright: %d\n", leftComm, rightComm);
  }
  // disconnect if no packet received in a while
  if (millis() - timeout > 500) {
    if (showWarning == false) {
      showWarning = true;
      Serial.println("disconnected");
    }
    leftComm = 0;
    rightComm = 0;
    leftSpeed = 0;
    rightSpeed = 0;
  } else {
    showWarning = false;
  }

  if (leftComm != 0) { // if we are accelerating
    if (leftSpeed > -INERTIA && leftSpeed < INERTIA) { // while speed is within [-INERTIA, INERTIA]
      leftSpeed += leftComm; // move speed in the direction we are changing
    }
  }
  else if (leftSpeed != 0) { // if we are stoppping, and not already stopped
    leftSpeed -= leftSpeed > 0 ? 1 : -1; // stop
  }
 
  if (rightComm != 0) { 
    if (rightSpeed > -INERTIA && rightSpeed < INERTIA) { 
      rightSpeed += rightComm; 
    }
  }
  else if (rightSpeed != 0) {
    rightSpeed -= rightSpeed > 0 ? 1 : -1; 
  }

  analogWrite(rightFwd, 0);
  analogWrite(leftFwd, 0);
  analogWrite(rightBwd, 0);
  analogWrite(leftBwd, 0);
   
  Serial.printf("speed\tleft: %d\tright: %d\n", leftComm, rightComm);

  delay(5);
}
