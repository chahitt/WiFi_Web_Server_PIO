#include <Arduino.h>
#include <WiFi.h>

#define redLedPin 23
#define greenLedPin 22
#define yellowLedPin 2

const char* ssid = "TP-Link_C91A";
const char* password = "56092011";

WiFiServer server(80);

void setup()
{
    Serial.begin(115200);
    pinMode(redLedPin, OUTPUT);
    pinMode(greenLedPin, OUTPUT);
    pinMode(yellowLedPin, OUTPUT);
    delay(10);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        digitalWrite(redLedPin, !digitalRead(redLedPin));
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connecté ");
    Serial.println("Adresse IP : ");
    Serial.println(WiFi.localIP());

    digitalWrite(redLedPin, LOW);
    digitalWrite(greenLedPin, HIGH);
    server.begin();
}
int value = 0;

void loop()
{
    WiFiClient client = server.available();

    if (client) {
        Serial.println("Nouveau client. ");
        String currentLine = "";
        while (client.connected()) {
            if (client.available()) {
                char c = client.read();
                Serial.write(c);
                if (c == '\n') {
                    if (currentLine.length() == 0) {
                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-type:text/html");
                        client.println();

                        client.print("cliquez <a href=\"/H\"> ici</a> Pour allumer la led jaune.<br>");
                        client.print("cliquez <a href=\"/L\"> ici</a> Pour eteindre la led jaune.<br>");

                        client.println();
                        break;
                    } else {
                        currentLine = "";
                    }
                } else if (c != '\r') {
                    currentLine += c;
                }
                if (currentLine.endsWith("GET /H")) {
                    digitalWrite(yellowLedPin, HIGH);
                }
                if (currentLine.endsWith("GET /L")) {
                    digitalWrite(yellowLedPin, LOW);
                }
            }
        }
        client.stop();
        Serial.println("Client déconnecté");
    }
}