#define serdebug
#ifdef serdebug
#define DebugPrint(...) {  Serial.print(__VA_ARGS__); }
#define DebugPrintln(...) {  Serial.println(__VA_ARGS__); }
#else
#define DebugPrint(...) { }
#define DebugPrintln(...) { }
#endif

#include <Ticker.h>
#include "cy_wifi.h"
#include "cy_ota.h"
#include "cy_weather.h"
#include "cy_mqtt.h"
#include "btn_led_tool.h"
#include "dht22_tool.h"


const char* gv_hostname = "gizwitsdht22";


Ticker senstick;
boolean gv_senstick;

void do_senstick() {
  gv_senstick = true;
}

void setup() {
  // put your setup code here, to run once:

#ifdef serdebug
  Serial.begin(115200);
#endif

  DebugPrintln("\n" + String(__DATE__) + ", " + String(__TIME__) + " " + String(__FILE__));

  set_rgb(255, 255, 255);
  delay(500);


  wifi_init(gv_hostname);
  delay(500);

  init_ota(gv_hostname);

  init_mqtt(gv_hostname);
  //add_subtopic("ATSH28/KE/ALT/TEMP/....", callback_mqtt1);

  set_rgb(0, 0, 0);
  delay(500);

  init_dht22();
  do_sensor();
  gv_senstick = false;
  senstick.attach(60, do_senstick);

  //setup button
  pinMode(btnpin, INPUT);
  attachInterrupt(btnpin, IntBtn, CHANGE);
  delay(500);
}

void loop() {
  // put your main code here, to run repeatedly:

  check_ota();

  if (gv_senstick == true) {

    do_sensor();

    gv_senstick = false;
  }

  LDRValue = analogRead(LDRPin);

  check_btn();

  set_leds();

  check_mqtt();

  delay(100);

}

void do_sensor() {
  set_rgb(0, 255, 0);
  get_dht22();
  set_rgb(0, 0, 0);

}

