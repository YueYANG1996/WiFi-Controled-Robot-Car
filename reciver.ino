/*
   receives packets from port 2390
   and port 2420
*/
#include <WiFi.h>
#include <WiFiUdp.h>
const char* ssid = "Mechatronics";
const char* password = "YayFunFun";
WiFiUDP UDPTestServer;
WiFiUDP UDPTestServer2;
int motor_left_1A = 21; //set the port for logic pins
int motor_left_2A = 19;
int motor_right_4A = 18;
int motor_right_3A = 17;
int motor_left_speed = 25; //set the speed control pins
int motor_right_speed = 26;
unsigned int UDPPort = 2420;//the port of the car
unsigned int UDPselfport = 2390; //the port to receive go command
IPAddress myIPaddress(192, 168, 1, 155); //the car ip address
const int packetSize = 255;// can be up to 65535
byte packetBuffer[packetSize];//packetBuffer for remote control
byte packetBuffer2[packetSize];//packetBuffer for Go! command
void setup() {
  ledcAttachPin(motor_left_speed, 1); // assign speed control pins to channels
  ledcAttachPin(motor_right_speed, 2);
  ledcSetup(1, 12000, 8); // 12 kHz PWM, 8-bit resolution
  ledcSetup(2, 12000, 8);
  pinMode(21, OUTPUT); //set the motor logic pins to output
  pinMode(19, OUTPUT);
  pinMode(18, OUTPUT);
  pinMode(17, OUTPUT);
  //std setup
  Serial.begin(115200);
  Serial.println();
  Serial.print("Connecting to "); Serial.println(ssid);
  WiFi.begin(ssid, password);
  //wifi setup
  WiFi.config(myIPaddress, IPAddress(192, 168, 1, 202), IPAddress(255, 255, 255, 0));
  UDPTestServer.begin(UDPPort);//remote controller
  UDPTestServer2.begin(UDPselfport);//go! command
  while (WiFi.status() != WL_CONNECTED) {
    //wait until connected
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
}
void loop() {
  //check for packet events
  handleUDPServer2();
  handleUDPServer();
  delay(1);
}
void forward()
{
  /*forward set speed to maximum
     1A high, 3A high
  */
  ledcWrite(1, 255);
  ledcWrite(2, 255);
  digitalWrite(motor_left_1A, HIGH);
  digitalWrite(motor_left_2A, LOW);
  digitalWrite(motor_right_3A, HIGH);
  digitalWrite(motor_right_4A, LOW);

}
void backward()
{
  /*backward set speed to maximum
    2A high, 4A high
  */
  ledcWrite(1, 255);
  ledcWrite(2, 255);
  digitalWrite(motor_left_1A, LOW);
  digitalWrite(motor_left_2A, HIGH);
  digitalWrite(motor_right_3A, LOW);
  digitalWrite(motor_right_4A, HIGH);

}
void left()
{
  /*left set speed to 200
     2A high, 3A high
  */
  ledcWrite(1, 200);
  ledcWrite(2, 200);
  digitalWrite(motor_left_1A, LOW);
  digitalWrite(motor_left_2A, HIGH);
  digitalWrite(motor_right_3A, HIGH);
  digitalWrite(motor_right_4A, LOW);

}
void right()
{
  /*right set speed to 200
    1A high, 4A high
  */
  ledcWrite(1, 200);
  ledcWrite(2, 200);
  digitalWrite(motor_left_1A, HIGH);
  digitalWrite(motor_left_2A, LOW);
  digitalWrite(motor_right_3A, LOW);
  digitalWrite(motor_right_4A, HIGH);

}
void STOP()
{
  /*
    all low
  */
  ledcWrite(1, 255);
  ledcWrite(2, 255);
  digitalWrite(motor_left_1A, LOW);
  digitalWrite(motor_left_2A, LOW);
  digitalWrite(motor_right_3A, LOW);
  digitalWrite(motor_right_4A, LOW);

}
void handleUDPServer() {
  //check for packets from remote controller
  int cb = UDPTestServer.parsePacket();
  if (cb) {
    UDPTestServer.read(packetBuffer, packetSize);
    String myData = "";
    for (int i = 0; i < packetSize; i++) {
      myData += (char)packetBuffer[i];
    }
    Serial.println (myData);
    if (myData == "F")
    {
      //if receiver F then go forward
      Serial.println ("forward");
      forward();

    }
    if (myData == "B")
    {
      //if receiver B then go backward
      Serial.println ("backward");
      backward();

    }
    if (myData == "L")
    {
      //if receiver L then go left
      Serial.println ("turnleft");
      left();

    }
    if (myData == "R")
    {
      //if receiver R then go right
      Serial.println ("turnright");
      right();

    }
    if (myData == "S")
    {
      //if receiver S then stop
      Serial.println ("STOP");
      STOP();
      STOP();
    }
  }
}
void handleUDPServer2() {
  //check packets for go command
  int cb = UDPTestServer2.parsePacket();
  if (cb) {
    UDPTestServer2.read(packetBuffer2, packetSize);
    String myData1 = "";
    for (int i = 0; i < packetSize; i++) {
      myData1 += (char)packetBuffer2[i];
    }
    Serial.println (myData1);
    if (myData1 == "GO!")
    {
      //if go command received, then go forward
      Serial.println ("forward");
      forward();

    }
  }
}
