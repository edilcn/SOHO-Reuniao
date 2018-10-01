#define THINGER_SERVER "172.100.100.22"

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ThingerESP8266.h>
#include <FS.h>
#include <time.h>
#include <ArduinoOTA.h>
#include <NtpClientLib.h>
#include <TimeLib.h>


// Credenciais do Thinger.IO
// #define USERNAME "paumito"
// #define DEVICE_ID "teste"
// #define DEVICE_CREDENTIAL "gAiu$u1wGJ9D"

#define USERNAME "SHLabs"
#define DEVICE_ID "reuniao_1"
#define DEVICE_CREDENTIAL "0vPtNF4Mjngi"

// Informações do WiFi
#define SSID_STA "soholabs"
#define SSID_PASSWORD "s0h0l@b5"

// Pinos no ESP
#define SALA_1  D0
#define SALA_2  D1
#define SALA_3  D2
#define SALA_4  D5
#define SALA_5  D6

ThingerESP8266 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);

//NTPVARIAVIS
int8_t timeZone = -3;
int8_t minutesTimeZone = 0;

bool Status;

/*------------------------------NTP Client -----------------------------------*/
void processSyncEvent (NTPSyncEvent_t ntpEvent) {
    if (ntpEvent) {
        Serial.print ("Time Sync error: ");
        if (ntpEvent == noResponse)
            Serial.println ("NTP server not reachable");
        else if (ntpEvent == invalidAddress)
            Serial.println ("Invalid NTP server address");
    } else {
        Serial.print ("Got NTP time: ");
        Serial.println (NTP.getTimeDateString (NTP.getLastNTPSync ()));
    }
}

boolean syncEventTriggered = false; // True if a time even has been triggered
NTPSyncEvent_t ntpEvent; // Last triggered event


void setup() {
    Serial.begin(115200);

    NTP.begin ("a.st1.ntp.br", timeZone, false, minutesTimeZone);
    NTP.setInterval (3600);

    NTP.onNTPSyncEvent ([](NTPSyncEvent_t event) {
          ntpEvent = event;
          syncEventTriggered = true;
      });

    pinMode(SALA_1, OUTPUT);
    pinMode(SALA_2, OUTPUT);
    pinMode(SALA_3, OUTPUT);
    pinMode(SALA_4, OUTPUT);
    pinMode(SALA_5, OUTPUT);

    digitalWrite(SALA_1, LOW);
    digitalWrite(SALA_2, LOW);
    digitalWrite(SALA_3, LOW);
    digitalWrite(SALA_4, LOW);
    digitalWrite(SALA_5, LOW);

    thing.add_wifi(SSID_STA, SSID_PASSWORD);

    thing["Sala_1"] = [](pson& in, pson& out){
      if(in.is_empty()){
        in = (bool) digitalRead(SALA_1);
        out = (bool) digitalRead(SALA_1);
      }
      else{
        digitalWrite(SALA_1, in ? HIGH : LOW);
      }
    };

    thing["Sala_2"] = [](pson& in, pson& out){
      if(in.is_empty()){
        in = (bool) digitalRead(SALA_2);
        out = (bool) digitalRead(SALA_2);
      }
      else{
        digitalWrite(SALA_2, in ? HIGH : LOW);
      }
    };

    thing["Sala_3"] = [](pson& in, pson& out){
      if(in.is_empty()){
        in = (bool) digitalRead(SALA_3);
        out = (bool) digitalRead(SALA_3);
      }
      else{
        digitalWrite(SALA_3, in ? HIGH : LOW);
      }
    };

    thing["Sala_4"] = [](pson& in, pson& out){
      if(in.is_empty()){
        in = (bool) digitalRead(SALA_4);
        out = (bool) digitalRead(SALA_4);
      }
      else{
        digitalWrite(SALA_4, in ? HIGH : LOW);
      }
    };

    thing["Sala_5"] = [](pson& in, pson& out){
      if(in.is_empty()){
        in = (bool) digitalRead(SALA_5);
        out = (bool) digitalRead(SALA_5);
      }
      else{
        digitalWrite(SALA_5, in ? HIGH : LOW);
      }
    };
}

void loop() {
    thing.handle();
    if ((WiFi.status() == WL_CONNECTED) && !(WiFi.localIP() == INADDR_NONE)){
      Status = true;
    }
    else Status = false;
    if (syncEventTriggered) {
          processSyncEvent (ntpEvent);
          syncEventTriggered = false;
    }
}
