/*
   receives packets from UDP-TCPsender.c (plus any other broadcasters)

*/
#include<WiFi.h>
#include<WiFiUdp.h>
#define MOTOR_FORWARD 19 //set the port for logic pins 
#define MOTOR_BACKWARD 18
#define MOTOR_LEFT 21
#define MOTOR_RIGHT 17
const char* ssid = "Mechatronics"; //ssid+passward
const char* password = "YayFunFun";
WiFiUDP UDPTestServer;
unsigned int udpTargetPort = 2420; //the target port of the car
unsigned int udplocalPort = 2600; //arbitary port for remote controller
const int UDP_PACKET_SIZE = 255;// can be up to 65535
char udpBuffer[UDP_PACKET_SIZE];
IPAddress ipTarget(192, 168, 1, 155);//the car ip address
void setup() {
  //std setup
  Serial.begin(115200);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  //wifi setup
  WiFi.config(IPAddress(192, 168, 1, 202),//the remote controller ip address
              IPAddress(192, 168, 1, 155),
              IPAddress(255, 255, 255, 0));
  UDPTestServer.begin(udplocalPort);
  while (WiFi.status() != WL_CONNECTED) {
    //wait until connected
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected as");
  Serial.print(WiFi.localIP());
  WiFi.mode(WIFI_STA);
}
void FORWARD()
{
  //send forward command F
  strcpy(udpBuffer, "F");
  // send what ever you want upto 48 char
  UDPTestServer.beginPacket(ipTarget, udpTargetPort);
  UDPTestServer.printf("%s", udpBuffer);
  UDPTestServer.endPacket();

  //Serial.println(udpBuffer);
}
void BACKWARD()
{
  //send backward command B
  strcpy(udpBuffer, "B");
  // send what ever you want upto 48 char
  UDPTestServer.beginPacket(ipTarget, udpTargetPort);
  UDPTestServer.printf("%s", udpBuffer);
  UDPTestServer.endPacket();
  //Serial.println(udpBuffer);
}
void LEFT()
{
  //send turn left command L
  strcpy(udpBuffer, "L");
  // send what ever you want upto 48 char
  UDPTestServer.beginPacket(ipTarget, udpTargetPort);
  UDPTestServer.printf("%s", udpBuffer);
  UDPTestServer.endPacket();
  //Serial.println(udpBuffer);
}
void RIGHT()
{
  //send turn right command L
  strcpy(udpBuffer, "R");
  // send what ever you want upto 48 char
  UDPTestServer.beginPacket(ipTarget, udpTargetPort);
  UDPTestServer.printf("%s", udpBuffer);
  UDPTestServer.endPacket();
  //Serial.println(udpBuffer);
}
void STOP()
{
  //send stop command S
  strcpy(udpBuffer, "S");
  // send what ever you want upto 48 char
  UDPTestServer.beginPacket(ipTarget, udpTargetPort);
  UDPTestServer.printf("%s", udpBuffer);
  UDPTestServer.endPacket();
  //Serial.println(udpBuffer);
}
// send udp packet each 5 seconds
void loop()
{
  /*if forward button is pressed, send forward
    if the button is released, send stop
  */
  if (digitalRead(MOTOR_FORWARD) == HIGH)
  {
    Serial.println("F");
    FORWARD();
    delay(100);
    if (digitalRead(MOTOR_FORWARD) == LOW)
    {
      Serial.println("S");
      STOP();
      STOP();
      delay(100);
    }
  }
  /*if BACKWARD button is pressed, send backward
    if the button is released, send stop
  */
  if (digitalRead(MOTOR_BACKWARD) == HIGH)
  {
    Serial.println("B");
    BACKWARD();
    delay(100);
    if (digitalRead(MOTOR_BACKWARD) == LOW)
    {
      Serial.println("S");
      STOP();
      STOP();
      delay(100);
    }
  }
  /*if LEFT button is pressed, send left
    if the button is released, send stop
  */
  if (digitalRead(MOTOR_LEFT) == HIGH)
  {
    Serial.println("L");
    LEFT();
    delay(100);
    if (digitalRead(MOTOR_LEFT) == LOW)
    {
      Serial.println("S");
      STOP();
      STOP();
      delay(100);
    }
  }
  /*if right button is pressed, send right
    if the button is released, send stop
  */
  if (digitalRead(MOTOR_RIGHT) == HIGH)
  {
    Serial.println("R");
    RIGHT();
    delay(100);
    if (digitalRead(MOTOR_RIGHT) == LOW)
    {
      Serial.println("S");
      STOP();
      STOP();
      delay(100);
    }
  }
}
