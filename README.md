# External Water Level Sensor – Apex Integration

Replace the Neptune Apex FMM's stock FS-IR02 optical level sensor with a DFRobot SEN0204 non-contact sensor that reads the water level through the container wall.

## How it works
The FS-IR02 is a 4-wire sensor. The FMM supplies +5V on white and blue, powers the sensor's internal IR LED through a 390 Ω resistor on red, and reads yellow as the output through a 4.7 kΩ pull-down. The output goes HIGH when wet and LOW when dry.

The SEN0204 also outputs HIGH when wet, so polarity matches. A PC817 optocoupler copies the SEN0204's output onto the FMM's yellow line and keeps the two circuits isolated.

## Parts
- DFRobot SEN0204 non-contact liquid level sensor (XKC-Y25-T12V)
- PC817 optocoupler (chip or module)
- 1 kΩ resistor
- 5V USB power supply for the sensor
- Optional: Arduino Nano R4 (ABX00143) for debounce/delay

## Wiring (no Arduino)
| From | To |
| --- | --- |
| SEN0204 VCC (red) | 5V supply + |
| SEN0204 GND (black) | 5V supply − and PC817 cathode (pin 2) |
| SEN0204 signal | 1 kΩ resistor, then PC817 anode (pin 1) |
| PC817 collector (pin 4) | FMM white (+5V) |
| PC817 emitter (pin 3) | FMM yellow (output) |
| FMM red and blue | Leave unconnected (or put 390 Ω across them if the FMM reports a missing sensor) |

Wet: the sensor goes HIGH, the PC817 turns on, and FMM yellow reads HIGH. Dry: the FMM's 4.7 kΩ pull-down holds yellow LOW.

## Optional: Arduino debounce
Power the Nano R4 from the same 5V supply. Connect the sensor signal to D7, and connect D8 through a 1 kΩ resistor to PC817 pin 1.

```cpp
const int sensorPin = 7;
const int outPin = 8;
const unsigned long holdMs = 3000; // reading must stay stable this long before it counts

bool state = false;
bool lastRead = false;
unsigned long changedAt = 0;

void setup() {
  pinMode(sensorPin, INPUT);
  pinMode(outPin, OUTPUT);
}

void loop() {
  bool wet = digitalRead(sensorPin) == HIGH;
  if (wet != lastRead) { lastRead = wet; changedAt = millis(); }
  if (wet != state && millis() - changedAt >= holdMs) state = wet;
  digitalWrite(outPin, state ? HIGH : LOW);
  delay(50);
}
```

## Setup and testing
1. Use a multimeter to confirm that FMM white carries +5V relative to ground before wiring.
2. Mount the SEN0204 on a non-metallic container wall at the target level, and adjust sensitivity with the knob on its adapter.
3. Dunk test: confirm the FMM input in Apex Fusion flips between dry and wet.
4. Only then let it control an ATO pump.

## Notes
- The FMM's 4 wires don't include a direct ground. Don't power the sensor from red, because it's in series with a 390 Ω resistor.
- Reference: [DFRobot FS-IR02 wiki](https://wiki.dfrobot.com/sen0205/docs/18360)
