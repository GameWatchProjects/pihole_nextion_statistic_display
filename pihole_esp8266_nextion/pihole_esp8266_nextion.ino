/**
 *  ESP8266 NodeMCU 3 & Nextion NX8048T070 Display 7 inch 
 *  Requirements: - Raspberry Pi Zero/A+/2 B/3 B/3 B+/4 B with a running Pihole >= 5 and NTP Serice installation
 *                - Nextion NX8048T070 display with a mico sd card for updating the display firmware
 *                - NodeMCU V3 with 4 wires (can be soldered direct with the Nextion NX8048T070 display connector)
 *                
 *                                    GND   ->    GND
 *                  NodeMCU           TX    ->    RX      Nextion Display
 *                  Connector         RX    ->    TX      Connector
 *                                    5V    ->    5V
 *                                    
 *                - Micro-USB 5V DC power adapter
 *                
 *                - Q: Why owen NTP Service, when yur ISP or Router have this service?
 *                  A: Your Router and/or your ISP will block you after few minutes, when you ask to often the time to synchronize your network time,
 *                     so we use our owen lokal timeserver and we can see on the display by the time, when was the last refresh successfully.
 *                     In most cases we didn't need realtime statistics, so the default refresh time of 10 seconds is enought.
 */

#include <Arduino.h>
#include <NTPClient.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiUdp.h>
#define USE_SERIAL Serial

WiFiClient client; // create wifi client object

const char* host         = "192.168.XXX.XXX";           // Pi-Hole IP - NOTICE: Make sure, the Pi-Hole and NTP service installation is on the same maschine
const char* ssid         = "YOUR-WIFI-SSID";            // Wi-Fi SSID Name
const char* password     = "YOUR-WIFI-PASS";            // Wi-Fi Password
const char* wifihost     = "pihole-display";            // Wi-Fi Hostname


// You need to adjust the UTC offset for your timezone in milliseconds
//    For UTC -5.00 : -5 * 60 * 60 = -18000
//    For UTC +1.00 : 1 * 60 * 60 = 3600
//    For UTC +0.00 : 0 * 60 * 60 = 0
const long utcOffsetInSeconds = 3600;

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, host, utcOffsetInSeconds); 


void setup() {
    // Deault Baudrate by the Nextion displays is 9600, but temporary the default baud rate 
    // will change temporary to 115200 by using the given nextion tft file
    Serial.begin(115200);
    delay(1000);
    Start_WiFi(ssid,password);
    timeClient.begin();
}


void loop() {
       
   if((WiFi.status() == WL_CONNECTED)) {
        timeClient.update();
        HTTPClient http;
        http.begin("http://"+ String(host) +"/admin/api.php?summary"); 
        int httpCode = http.GET();
        if(httpCode > 0) {
            if(httpCode == HTTP_CODE_OK) {
                String payload = http.getString();       // save as string 'payload'
                const size_t bufferSize = JSON_OBJECT_SIZE(10) + 250;
                DynamicJsonBuffer jsonBuffer(bufferSize);
                JsonObject& root = jsonBuffer.parseObject(payload);
                JsonObject& response = root["response"];
                JsonObject& response_data0 = response["data"][0];
                const char* domains_being_blocked = root["domains_being_blocked"]; 
                const char* dns_queries_today = root["dns_queries_today"]; 
                const char* ads_blocked_today = root["ads_blocked_today"]; 
                const char* ads_percentage_today = root["ads_percentage_today"]; 
                const char* unique_domains = root["unique_domains"]; 
                const char* queries_forwarded = root["queries_forwarded"]; 
                const char* queries_cached = root["queries_cached"]; 
                const char* clients_ever_seen = root["clients_ever_seen"]; 
                const char* unique_clients = root["unique_clients"]; 
                const char* status_pihole = root["status"];
                const char* update_blocklist_days = root["gravity_last_updated"]["relative"]["days"];
                const char* update_blocklist_hours = root["gravity_last_updated"]["relative"]["hours"];
                const char* update_blocklist_minutes = root["gravity_last_updated"]["relative"]["minutes"];

                         
                String command1 = "start_page.ads.txt=\"" + String(dns_queries_today) + "\"";
                Serial.print(command1);
                endNextionCommand();
                delay(10);
                String command2 = "start_page.clients.txt=\"(Clients: unique: " + String(unique_clients) + " / ever seen: " + String(clients_ever_seen) + ")\"";
                Serial.print(command2);
                endNextionCommand();
                delay(10);
                String command3 = "start_page.domains.txt=\"" + String(domains_being_blocked) + "\"";
                Serial.print(command3);
                endNextionCommand();
                delay(10);
                String command4 = "start_page.today.txt=\"" + String(ads_percentage_today) + "%\"";
                Serial.print(command4);
                endNextionCommand();
                delay(10);
                String command5 = "start_page.blocked.txt=\"" + String(ads_blocked_today) + "\"";
                Serial.print(command5);
                endNextionCommand();
                delay(10);
                String command6 = "start_page.update.txt=\"(Last update: " + String(update_blocklist_days) + " days / " + String(update_blocklist_hours) + " hours / " + String(update_blocklist_minutes) + " minutes)\"";
                Serial.print(command6);
                endNextionCommand();
                delay(10);
                                
                if (String(status_pihole) == "enabled") {
                   String command7 = "start_page.status.pic=2";
                   Serial.print(command7);
                   endNextionCommand();
                   delay(10);
                   String command8 = "start_page.status.txt=\"" + String(timeClient.getFormattedTime()) + "\"";
                   Serial.print(command8);
                   endNextionCommand();
                   delay(10);
 
                }
                else {
                   String command9 = "start_page.status.pic=1";
                   Serial.print(command9);
                   endNextionCommand();
                   delay(10);
                   String command10 = "start_page.status.txt=\"" + String(timeClient.getFormattedTime()) + "\"";
                   Serial.print(command10);
                   endNextionCommand();
                   delay(10);

                }
                
                http.end();
            }
        } else {
                   String command11 = "start_page.status.txt=\"No WiFi\"";
                   Serial.print(command11);
                   endNextionCommand();
       }
    }
        delay(10000); // 10 sec. refresh delay
}


void endNextionCommand(){
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);
}


int Start_WiFi(const char* ssid, const char* password){
    int connAttempts = 0;
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED ) {
      WiFi.hostname(wifihost);
        delay(500);
        if(connAttempts > 20) return -5;
        connAttempts++;
        }
    return 1;
}
