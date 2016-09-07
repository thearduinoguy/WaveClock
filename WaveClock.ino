//#include <Arduino.h>
#include <IRremote.h>
#include "LedControl.h"
#include <Wire.h>
#include "RTClib.h"

int RECV_PIN = 6;
#define IR_LED      5

LedControl lc = LedControl(12, 11, 10, 1);
RTC_DS1307 rtc;

IRsend irsend;
IRrecv irrecv(RECV_PIN);

decode_results results;  // For storing receiered IR packets

unsigned long lastMillis;

void setup() {


        Serial.begin(9600);
        delay(1000);

        if (!rtc.begin()) {
                Serial.println("Couldn't find RTC");
                while (1) ;
        }

        if (!rtc.isrunning()) {
                Serial.println("RTC is NOT running!");
        }

        //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

        lc.shutdown(0, false);

        lc.setIntensity(0, 8);

        lc.clearDisplay(0);

        irrecv.enableIRIn();  // Start the receiver
}

void setDigits(int number, int seconds) {

        lc.setDigit(0, 3, number%10, false);
        number=number/10;
        lc.setDigit(0, 2, number%10, false);
        number=number/10;
        if (seconds%2) lc.setDigit(0, 1, number%10, true);
        else lc.setDigit(0, 1, number%10, false);
        number=number/10;
        lc.setDigit(0, 0, number%10, false);

}

void loop() {

        DateTime now = rtc.now();
        int hour = now.hour();
        int minute = now.minute();
        int timeNow=(hour*100)+minute;
        int seconds=now.second();

        irsend.enableIROut(38);
        irsend.mark(0);

        if (!digitalRead(RECV_PIN))
        {
                lc.shutdown(0, false);
                lastMillis=millis();
        }
        setDigits(timeNow, seconds);
        if ((millis()-lastMillis)>=2000) lc.shutdown(0, true);
}
