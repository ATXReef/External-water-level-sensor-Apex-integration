/*
 * ApexLevelDebounce.ino
 * Arduino Nano R4 (ABX00143): reads a DFRobot SEN0204 non-contact level sensor
 * and drives a PC817 optocoupler that stands in for the FS-IR02 on an Apex FMM.
 *
 * Wiring:
 *   SEN0204 signal -> D7
 *   D8 -> 1k resistor -> PC817 pin 1 (anode); PC817 pin 2 -> GND
 *   PC817 pin 4 (collector) -> FMM white (+5V); pin 3 (emitter) -> FMM yellow
 * Logic: wet = HIGH (same as FS-IR02). Built-in LED mirrors the output state.
 */

const int SENSOR_PIN = 7;
const int OUT_PIN = 8;
const unsigned long HOLD_MS = 3000;  // reading must stay stable this long before it counts

bool state = false;
bool lastRead = false;
unsigned long changedAt = 0;

void setup() {
  Serial.begin(9600);
  pinMode(SENSOR_PIN, INPUT);
  pinMode(OUT_PIN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  state = lastRead = (digitalRead(SENSOR_PIN) == HIGH);
  changedAt = millis();
}

void loop() {
  bool wet = digitalRead(SENSOR_PIN) == HIGH;
  if (wet != lastRead) {
    lastRead = wet;
    changedAt = millis();
  }
  if (wet != state && millis() - changedAt >= HOLD_MS) {
    state = wet;
    Serial.println(state ? "WET" : "DRY");
  }
  digitalWrite(OUT_PIN, state ? HIGH : LOW);
  digitalWrite(LED_BUILTIN, state ? HIGH : LOW);
  delay(50);
}
