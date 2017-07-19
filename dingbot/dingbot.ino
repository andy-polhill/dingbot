#include <Homie.h>
#include <Adafruit_NeoPixel.h>

#define LED_PIN D2

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(1, LED_PIN, NEO_GRB + NEO_KHZ800);

const int INTERVAL = 10;

const String STARTED = "started";
const String PASSED = "passed";
const String FAILED = "failed";
const String UNKNOWN = "unknown";


String state = "unknown";
bool blink = true;

unsigned long blinkTime = 0;

HomieNode lightNode("light", "switch");

void onHomieEvent(const HomieEvent& event) {
switch(event.type) {
    case HomieEventType::CONFIGURATION_MODE:
      Serial.write("Config mode");
      pixels.setPixelColor(0, pixels.Color(50, 0, 0));
      break;
    case HomieEventType::NORMAL_MODE:
      Serial.write("Normal mode");
      pixels.setPixelColor(0, pixels.Color(0, 50, 0));
      break;
    case HomieEventType::WIFI_CONNECTED:
      Serial.write("Wifi Connected");
      pixels.setPixelColor(0, pixels.Color(0, 150, 0));
      break;
    case HomieEventType::WIFI_DISCONNECTED:
      Serial.write("Wifi Disconnected");
      pixels.setPixelColor(0, pixels.Color(50, 0, 0));
      break;
    case HomieEventType::MQTT_CONNECTED:
      Serial.write("MQTT Connected");
      pixels.setPixelColor(0, pixels.Color(255, 255, 255));
      break;
  }
  pixels.show();
}

bool lightHandler(const HomieRange& range, const String& value) {
  Serial.write(value.c_str());
  lightNode.setProperty("state").send(value);
  state = value;
  if (strcmp(value.c_str(), UNKNOWN.c_str()) == 0)  {
    pixels.setPixelColor(0, pixels.Color(0, 0, 255));
  } else if (strcmp(value.c_str(), STARTED.c_str()) == 0) {
    pixels.setPixelColor(0, pixels.Color(0, 0, 255));
  } else if (strcmp(value.c_str(), PASSED.c_str()) == 0) {
    pixels.setPixelColor(0, pixels.Color(0, 255, 0));
  } else if (strcmp(value.c_str(), FAILED.c_str()) == 0) {
    pixels.setPixelColor(0, pixels.Color(255, 0, 0));
  } else {
    pixels.setPixelColor(0, pixels.Color(255, 255, 255));
  }
  pixels.show();
  return true;
}

void loopHandler() {
  if(strcmp(state.c_str(), STARTED.c_str()) == 0) {
    unsigned long time = millis() - blinkTime;
    if(time > 500) {
      blinkTime = millis();
      if(blink) {
        pixels.setPixelColor(0, pixels.Color(0, 0, 255));
        blink = false;
      } else {
        pixels.setPixelColor(0, pixels.Color(0, 0, 0));        
        blink = true;
      }
      pixels.show();
    }
  }
  return;
}

void setup() {
  Serial.begin(115200);
  Serial << endl << endl;

  pixels.begin();

  Homie_setFirmware("dingbot", "1.0.0");

  lightNode.advertise("state").settable(lightHandler);
  Homie.onEvent(onHomieEvent);

  Homie.setLoopFunction(loopHandler);
  Homie.setup();
}

void loop() {
  Homie.loop();
}
