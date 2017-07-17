#include <Homie.h>

const int INTERVAL = 10;

unsigned long lastPoll = 0;

HomieNode bellNode("bell", "bell");

void setupHandler() {
  bellNode.setProperty("ding").send("dong");
}

void loopHandler() {
  if (millis() - lastPoll >= INTERVAL * 1000UL || lastPoll == 0) {
    bellNode.setProperty("ding").send("dong");
    Serial.println('GO');
    lastPoll = millis();
  }
}

void setup() {
  Serial.begin(115200);
  Serial << endl << endl;
  Homie_setFirmware("dingbot", "1.0.0");
  Homie.setSetupFunction(setupHandler).setLoopFunction(loopHandler);

  bellNode.advertise("ding");

  Homie.setup();
}

void loop() {
  Homie.loop();
}
