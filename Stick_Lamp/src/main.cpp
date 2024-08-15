#define PIN D7
#define NUM_LEDS 64

//#define AP_SSID "WikiLink-AC3D"
//#define AP_PASS "f15cbb4e"

#define AP_SSID "iPhone (Андрей)"
#define AP_PASS "refobu8895"

#include <iostream>
#include <GyverHub.h>
#include "Adafruit_NeoPixel.h"
#include "Color_set.h"
#include <Pairs.h>

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);
GyverHub hub("MyDevice", "ESP8266", ""); // имя сети, имя устройства, иконка

int Click_Pwr_btn;
int Power_Status;
Pairs Brightness_data;
int Color_count = 0; // Текстовая переменная для выбора элемента массива
bool flag = 0;   // Переменная обработка флага прерывания
int Brightness = 125;
void fill_start()
{
  strip.setBrightness(Brightness);
  strip.clear();
  for (int a = 0; a < NUM_LEDS; a++)
  {
    
    strip.setPixelColor(a, colors[Color_count]);
    strip.show();
    delay(10);
  }
  flag = 1;
  Power_Status = 1;
}

void power_on()
{
  Click_Pwr_btn = 1;
    if (Power_Status == Click_Pwr_btn)
  {
    loop();
  }
  else
  {
    fill_start();
  }
}

void power_off()
{
  Click_Pwr_btn = 0;
  Serial.println(Click_Pwr_btn);
  if (Power_Status == Click_Pwr_btn)
  {
    loop();
  }
  else
  {
    strip.clear();
    strip.show();
    Power_Status = 0;
  }
}

void back_color()
{
  if (Color_count == 0)
  {
    fill_start();
  }
  else
  {
    Color_count = Color_count - 1;
    fill_start();
  }
}

void next_color()
{
  if (Color_count == 10)
  {
    fill_start();
  }
  else
  {
    Color_count = Color_count + 1;
    fill_start();
  }
}

void Br_down()
{
  if (Brightness == 0)
  {
    fill_start();
  }
  else
  {
    Brightness = Brightness - 10;
    fill_start();
  }
}

void Br_up()
{
  if (Brightness == 250)
  {
    fill_start();
  }
  else
  {
    Brightness = Brightness + 10;
    fill_start();
  }
}

// билдер
void build(gh::Builder &b)
{
  b.Title("Control Panel");
  b.beginRow(); // начать
  b.Button_("PowerOn").icon("f011").color(gh::Colors::Green).attach(power_on);
  b.Button_("PowerOff").icon("f011").color(gh::Colors::Red).attach(power_off);
  b.endRow();

  b.Title("Brithness Set");
  b.beginRow();
  b.Button_("Br_down").icon("f137").color(gh::Colors::Yellow).attach(Br_down);
  b.Button_("Br_up").icon("f138").color(gh::Colors::Yellow).attach(Br_up); 
  b.endRow();

  b.Title("Color Set");
  b.beginRow();
  b.Button_("Color_Back").icon("f053").color(gh::Colors::Blue).attach(back_color);
  b.Button_("Color_Next").icon("f054").color(gh::Colors::Blue).attach(next_color);
  b.endRow();
}


void setup()
{
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  WiFi.begin(AP_SSID, AP_PASS);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print("Connecting... ");
  }
  Serial.println();
  Serial.println(WiFi.localIP());

  strip.begin();
  

  hub.config(F("MyDevices"), F("Stick_Lapm_1"), F(""));
  hub.onBuild(build); // подключаем билдер
  hub.begin();
}


void loop()
{
  hub.tick();

  if (flag == 0)
  {
    fill_start();
  }

  static gh::Timer tmr(1000); // период 1 секунда
  if (tmr)
  {
    hub.update(F("title")).value(millis());
  }
}
