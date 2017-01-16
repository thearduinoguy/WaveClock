#include <IRremoteESP8266.h>
#include "LedControl.h"
#include <ESP8266WiFi.h>
#include "TimeClient.h"
#include <WiFiUdp.h>

/*
   pin D8 is connected to the DataIn
   pin D7 is connected to the CLK
   pin D6 is connected to LOAD
   We have only a single MAX72XX.
 */
LedControl lc=LedControl(D8,D7,D6,1);

int RECV_PIN = D2;  // D4
IRsend irsend(0); //an IR led is connected to GPIO pin 0 (D3)
IRrecv irrecv(RECV_PIN);

decode_results results;  // For storing received IR packets

boolean tick = false;
unsigned long lastUpdate = millis();
unsigned long lastSecond = millis();
unsigned long lastMillis;
unsigned long lastTransmit;

String hours, minutes, seconds;
int currentSecond, currentMinute, currentHour;

char ssid[] = "xxxx";  //  your network SSID (name)
char pass[] = "xxxx";       // your network password

const float UTC_OFFSET = 0;
TimeClient timeClient(UTC_OFFSET);

void setup() {
        Serial.begin(115200);

        Serial.println();
        Serial.println();

        irsend.begin();
        irrecv.enableIRIn();  // Start the receiver

        lc.shutdown(0,false);
        /* Set the brightness to a medium values */
        lc.setIntensity(0,2);
        /* and clear the display */
        lc.clearDisplay(0);
        // We start by connecting to a WiFi network
        Serial.print("Connecting to ");
        Serial.println(ssid);
        WiFi.begin(ssid, pass);
        WiFi.begin(ssid, pass);

        while (WiFi.status() != WL_CONNECTED) {
                delay(500);
                Serial.print(".");
        }
        Serial.println();
        Serial.println("WiFi connected");
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());
        timeClient.updateTime();
        updateTime();
        lastUpdate = millis();
        lastSecond = millis();
        lastTransmit = millis();
        irsend.enableIROut(38);
}

void updateTime() {
        hours = timeClient.getHours();
        minutes = timeClient.getMinutes();
        seconds = timeClient.getSeconds();
        currentHour = hours.toInt();
        if (currentHour>12) currentHour -= 12;
        currentMinute = minutes.toInt();
        currentSecond = seconds.toInt();
        lastUpdate = millis();
}

void loop()
{
//  lc.setDigit(0,3,i,false);
        //
        if ((millis() - lastUpdate) > 1800000) updateTime();

        if ((millis() - lastSecond) > 1000)
        {
                tick = !tick;
                lc.clearDisplay(0);
                lastSecond = millis();
                currentSecond++;
                if (currentSecond > 59)
                { currentSecond = 0;
                  currentMinute++;
                  if (currentMinute > 59) {
                          currentMinute = 0;
                          currentHour++;
                          if (currentHour > 12) currentHour = 0;
                  }}
                String currentTime = "  " + String(currentHour) + ':' + String(currentMinute) + ':' + (currentSecond > 9 ? "" : "0") + String(currentSecond);
                Serial.println(currentTime);

                if (currentHour>12) lc.setDigit(0,0,int(currentHour/10),false);
                lc.setDigit(0,1,int(currentHour%10),(tick ? true : false));
                lc.setDigit(0,2,int(currentMinute/10),false);
                lc.setDigit(0,3,int(currentMinute%10),false);
        }

        if ((millis()-lastTransmit)>200)
        {
                irsend.sendSony(0xA90, 12);
                lastTransmit = millis();
        }

        if (irrecv.decode(&results)) {
                if(results.value==0xA093D67D);
                {
                        lc.shutdown(0, false);
                        lastMillis=millis();
                        Serial.println("Received!");
                }

                irrecv.resume(); // Receive the next value
        }

        if ((millis()-lastMillis)>=5000) lc.shutdown(0, true);
}
