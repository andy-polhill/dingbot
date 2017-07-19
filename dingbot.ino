#include <Homie.h>
#include <Adafruit_NeoPixel.h>

#define LED_PIN D2

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(1, LED_PIN, NEO_GRB + NEO_KHZ800);

const int INTERVAL = 10;
unsigned long lastPoll = 0;

HomieNode lightNode("light", "switch");

void onHomieEvent(const HomieEvent& event) {
switch(event.type) {
    case HomieEventType::CONFIGURATION_MODE:
      pixels.setPixelColor(0, pixels.Color(20, 20, 20));
      break;
    case HomieEventType::NORMAL_MODE:
      pixels.setPixelColor(0, pixels.Color(50, 50, 50));
      break;
    case HomieEventType::WIFI_CONNECTED:
      pixels.setPixelColor(0, pixels.Color(150, 150, 150));
      break;
    case HomieEventType::WIFI_DISCONNECTED:
      pixels.setPixelColor(0, pixels.Color(255, 0, 0));
      break;
    case HomieEventType::MQTT_CONNECTED:
      pixels.setPixelColor(0, pixels.Color(0, 255, 0));
      break;
  }
  pixels.show();
}

void lightOnHandler(const HomieRange& range, const String& value) {
  if (strcmp(value, "UNKNOWN") == 0)  {
      pixels.setPixelColor(0, pixels.Color(255, 255, 255));
  } else if (strcmp(value, "BUILDING") == 0) {
      pixels.setPixelColor(0, pixels.Color(255, 239, 213));    
  } else if (strcmp(value, "SUCCESS") == 0) {
      pixels.setPixelColor(0, pixels.Color(0, 255, 0));    
  } else if (strcmp(value, 'FAIL') == 0) {
      pixels.setPixelColor(0, pixels.Color(255, 0, 0));    
  }
}

void setup() {
  Serial.begin(115200);
  Serial << endl << endl;

  pixels.begin();

  Homie_setFirmware("dingbot", "1.0.0");
  lightNode.settable(lightOnHandler);
  Homie.onEvent(onHomieEvent);
  Homie.setup();
}

void loop() {
  Homie.loop();
}
