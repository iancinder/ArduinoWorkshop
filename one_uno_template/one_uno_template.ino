/*
  ONE-UNO STARTER TEMPLATE
  ------------------------
  Hardware: 1 Arduino Uno + male-male jumper wires. Nothing else.

  Project ideas:
    - Morse code messenger: type in the Serial Monitor, the LED blinks it.
    - Conductivity tester:  jumpers in A0 and GND are probes. Touch them to
                            things; a LOW A0 reading means it conducts well.
    - Ghost detector:       a loose jumper in A0 is an antenna. Wave your hand
                            near it and watch Tools > Serial Plotter.

  Pins this template sets up:
    - LED:   the built-in L LED (pin 13)
    - Key:   jumper in pin 2, tap the free end into any GND hole
    - Probe: jumper in A0

  As-is, this sketch is a self-test: the LED lights while the key touches
  GND, and the A0 reading prints 4 times a second (Serial Monitor at 9600).

  Using Claude? Paste this file in and say: "I have an Arduino Uno and
  jumper wires only. Starting from this template, build a ..."
*/

const int LED_PIN   = LED_BUILTIN;   // pin 13
const int KEY_PIN   = 2;
const int PROBE_PIN = A0;

// True while the key jumper is touching GND.
bool keyDown() {
  return digitalRead(KEY_PIN) == LOW;
}

// Turn the LED on for `ms` milliseconds, then off.
void blinkFor(int ms) {
  digitalWrite(LED_PIN, HIGH);
  delay(ms);
  digitalWrite(LED_PIN, LOW);
}

// Read A0 (0 to 1023).
//   pullUp = true  -> conductivity tester (good conductors read low)
//   pullUp = false -> ghost detector (the pin floats and picks up noise)
int readProbe(bool pullUp) {
  pinMode(PROBE_PIN, pullUp ? INPUT_PULLUP : INPUT);
  return analogRead(PROBE_PIN);
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(KEY_PIN, INPUT_PULLUP);
  Serial.begin(9600);
  Serial.println(F("One-Uno template running. Tap pin 2's jumper into GND to test the key."));
}

void loop() {
  // ---- Self-test demo: replace everything below with your project ----
  digitalWrite(LED_PIN, keyDown() ? HIGH : LOW);

  static unsigned long lastPrint = 0;
  if (millis() - lastPrint >= 250) {
    lastPrint = millis();
    Serial.print(F("A0:"));                 // "label:value" works in the Serial Plotter
    Serial.println(readProbe(false));
  }
}
