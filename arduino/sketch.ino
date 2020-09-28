#include "SoftwareSerial.h"
const byte rxPin = 3; // Wire this to Tx Pin of ESP8266
const byte txPin = 2; // Wire this to Rx Pin of ESP8266

const byte enA = 10;
const byte enB = 5;
const byte in1 = 9;
const byte in2 = 8;
const byte in3 = 7;
const byte in4 = 6;

SoftwareSerial esp8266(rxPin, txPin);

#define DEBUG true

String WIFI_SSID = "SSID";
String WIFI_PASS = "PASSWORD";

void setup()
{
    pinMode(enA, OUTPUT);
    pinMode(enB, OUTPUT);
    pinMode(in1, OUTPUT);
    pinMode(in2, OUTPUT);
    pinMode(in3, OUTPUT);
    pinMode(in4, OUTPUT);

    Serial.begin(9600);
    esp8266.begin(115200);

    esp8266Serial("AT+RST\r\n", 5000, DEBUG);
    esp8266Serial("AT+CWMODE=3\r\n", 5000, DEBUG);

    esp8266Serial("AT+CIPMUX=1\r\n", 5000, DEBUG);
    esp8266Serial("AT+CWJAP=\"" + WIFI_SSID + "\",\"" + WIFI_PASS + "\"\r\n", 5000, DEBUG);
    esp8266Serial("AT+CIFSR\r\n", 1000, DEBUG);
    esp8266Serial("AT+CIPSERVER=1,80\r\n", 5000, DEBUG);
}

void loop()
{
    MotorSpeed(225);

    if (esp8266.available())
    {
        if (esp8266.find("+IPD,"))
        {
            String msg;
            esp8266.find("?");
            msg = esp8266.readStringUntil(' ');
            Serial.println("RECIEVED:");
            Serial.println(msg);
            String command1 = msg.substring(0, 3);
            String command2 = msg.substring(4);

            if (DEBUG)
            {
                Serial.println(command1);
                Serial.println(command2);
            }
            delay(100);

            if (command1 == "mov")
            {

                if (command2 == "=1")
                {
                    MoveRight();
                }
                if (command2 == "=2")
                {
                    MoveLeft();
                }
                if (command2 == "=3")
                {
                    MoveFwd();
                }
                if (command2 == "=4")
                {
                    MoveBwd();
                }
                delay(1000);
                TurnOffMotors();
            }
        }
    }
}

String esp8266Serial(String command, const int timeout, boolean debug)
{
    String response = "";
    esp8266.print(command);
    Serial.print("Sending command: " + command);
    long int time = millis();
    while ((time + timeout) > millis())
    {
        while (esp8266.available())
        {
            char c = esp8266.read();
            response += c;
        }
    }
    if (debug)
    {
        Serial.print(response);
    }
    return response;
}

void MotorSpeed(int s)
{
    digitalWrite(enA, s);
    digitalWrite(enB, s);
}

void TurnOffMotors()
{
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
}

void MoveRight()
{
    MoveMotorA(true);
}

void MoveLeft()
{
    MoveMotorB(true);
}

void MoveFwd()
{
    MoveMotorA(true);
    MoveMotorB(true);
}

void MoveBwd()
{
    MoveMotorA(false);
    MoveMotorB(false);
}

void MoveMotorA(bool fwd)
{
    if (fwd)
    {
        digitalWrite(in1, HIGH);
        digitalWrite(in2, LOW);
    }
    else
    {
        digitalWrite(in1, LOW);
        digitalWrite(in2, HIGH);
    }
}
void MoveMotorB(bool fwd)
{
    if (fwd)
    {
        digitalWrite(in3, HIGH);
        digitalWrite(in4, LOW);
    }
    else
    {
        digitalWrite(in3, LOW);
        digitalWrite(in4, HIGH);
    }
}