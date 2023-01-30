#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <ESP8266WebServer.h>

//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager

#define TRACK_PIN D1

void setConfig();

ESP8266WebServer server(80);


void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);

    pinMode(TRACK_PIN, OUTPUT);

    //WiFiManager
    //Local intialization. Once its business is done, there is no need to keep it around
    WiFiManager wifiManager;
    //reset saved settings
    //wifiManager.resetSettings();

    //set custom ip for portal
    //wifiManager.setAPStaticIPConfig(IPAddress(10,0,1,1), IPAddress(10,0,1,1), IPAddress(255,255,255,0));

    //fetches ssid and pass from eeprom and tries to connect
    //if it does not connect it starts an access point with the specified name
    //here  "AutoConnectAP"
    //and goes into a blocking loop awaiting configuration
    wifiManager.autoConnect("T-Gauge-Server-AP");
    //or use this for auto generated name ESP + ChipID
    //wifiManager.autoConnect();

    // wait for wifi
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    WiFi.hostname("T-Gauge-Server");
    Serial.println("WiFi connected");

    // Print the IP address
    Serial.println(WiFi.localIP());

    server.on("/", setConfig);

    // Start the server
    server.begin();
    Serial.println("Web Server started");

    while (!Serial)  // Wait for the serial connection to be established.
        delay(50);
    Serial.print("T-Gauge-Server started");
}

void loop() {
    server.handleClient();
}

void setConfig() {
    Serial.println("Set Config " + server.arg("speed"));
    if (server.hasArg("speed") == true) {
        Serial.println("speed there");
        int speed = server.arg("speed").toInt();
        Serial.println(speed);
        Serial.println(speed>0);
        if (speed > 0) {
            if(speed>255) {
                speed = 255;
                Serial.println("Cannot raise Speed higher than 255.");
            } 
            Serial.println("Speed: " + speed);
        } else {
            Serial.println("Speed: 0");
            speed = 0;
        }

        analogWrite(D0, speed);
        server.send(200, "text/plain", "Speed set to: " + String(speed));
    } else {
        server.send(200, "text/plain", "no arg");
    }
}
