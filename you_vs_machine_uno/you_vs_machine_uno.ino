/*
  YOU vs. THE MACHINE  (Arduino Uno)
  ----------------------------------
  Upload this sketch, then open the Serial Monitor at 9600 baud.

  WIRING FOR ROUND 1 (you):
    Jumper A: Uno GND   -> breadboard rail
    Jumper B: Uno pin 2 -> leave the other end free. This is your "key".
    When the L LED on the board lights up, tap the free end of
    Jumper B onto the GND rail as fast as you can.

  WIRING FOR ROUND 2 (the machine):
    Take the free end of Jumper B off the rail and plug it into pin 12.
    The Uno notices the new wiring and starts on its own.

  Safety: pin 12 only ever pulls LOW (it never outputs 5V), so a
  wiring mistake on these pins can't short anything.
  Press the Uno's RESET button at any time to start over.
*/

const int LED_PIN     = 13;   // the built-in "L" LED
const int KEY_PIN     = 2;    // your key
const int MACHINE_PIN = 12;   // the Uno's own "finger" in round 2
const int TRIES       = 5;

float humanAvgMs = 0;

// ---------------- helpers ----------------

bool keyDown() {
  return digitalRead(KEY_PIN) == LOW;
}

// Wait until the key has stayed released for 50 ms (ignores contact bounce).
void waitForRelease() {
  unsigned long t = millis();
  while (millis() - t < 50) {
    if (keyDown()) t = millis();
  }
}

void waitForTap() {
  while (!keyDown()) {}
  waitForRelease();
}

// Machine pin: "release" = disconnected, "pull" = connect to GND.
void machineRelease() {
  pinMode(MACHINE_PIN, INPUT);        // also turns its pull-up off
}
void machinePull() {
  digitalWrite(MACHINE_PIN, LOW);
  pinMode(MACHINE_PIN, OUTPUT);
}

// True when pin 12 is wired to pin 2 (pin 2 follows what pin 12 does).
bool machineWired() {
  machineRelease();
  delayMicroseconds(100);
  bool highWhenReleased = (digitalRead(KEY_PIN) == HIGH);
  machinePull();
  delayMicroseconds(100);
  bool lowWhenPulled = (digitalRead(KEY_PIN) == LOW);
  machineRelease();
  return highWhenReleased && lowWhenPulled;
}

// ---------------- round 1: you ----------------

void humanRound() {
  Serial.println();
  Serial.println(F("============ ROUND 1: YOU ============"));
  Serial.println(F("When the L LED lights up, tap your key onto GND as fast as you can."));
  Serial.println(F("Tap your key once to start."));
  waitForTap();
  delay(500);

  unsigned long total = 0;
  unsigned long best = 99999;
  int tryNum = 1;

  while (tryNum <= TRIES) {
    Serial.print(F("Try "));
    Serial.print(tryNum);
    Serial.print(F(": wait for the LED... "));

    // Random wait. Tapping during it is a false start.
    unsigned long wait = random(1500, 4000);
    unsigned long start = millis();
    bool early = false;
    while (millis() - start < wait) {
      if (keyDown()) { early = true; break; }
    }
    if (early) {
      Serial.println(F("TOO EARLY! False start, this try is repeated."));
      waitForRelease();
      delay(1000);
      continue;
    }

    // GO!
    digitalWrite(LED_PIN, HIGH);
    unsigned long t0 = millis();
    while (!keyDown() && millis() - t0 < 3000) {}
    unsigned long ms = millis() - t0;
    digitalWrite(LED_PIN, LOW);

    if (ms >= 3000) {
      Serial.println(F("no tap within 3 seconds, this try is repeated."));
      delay(1000);
      continue;
    }

    Serial.print(ms);
    Serial.println(F(" ms"));
    total += ms;
    if (ms < best) best = ms;
    tryNum++;

    waitForRelease();
    delay(700);
  }

  humanAvgMs = total / (float)TRIES;
  Serial.println(F("--------------------------------------"));
  Serial.print(F("YOUR AVERAGE: "));
  Serial.print(humanAvgMs, 0);
  Serial.print(F(" ms   (best: "));
  Serial.print(best);
  Serial.println(F(" ms)"));
}

// ---------------- round 2: the machine ----------------

void machineRound() {
  Serial.println();
  Serial.println(F("========= ROUND 2: THE MACHINE ========="));
  Serial.println(F("Now let the Uno race itself:"));
  Serial.println(F("  1. Take the free end of your key jumper off GND."));
  Serial.println(F("  2. Plug it into pin 12 (so pin 2 is wired to pin 12)."));
  Serial.println(F("Waiting for you to rewire..."));

  while (!machineWired()) delay(200);
  delay(500);
  Serial.println(F("Wired up! The Uno will signal itself and react, 5 times:"));

  unsigned long total = 0;
  unsigned long fastest = 999999;
  unsigned long slowest = 0;
  int tryNum = 1;

  while (tryNum <= TRIES) {
    machineRelease();
    delay(random(300, 800));

    noInterrupts();                       // nothing else may interrupt the race
    unsigned long t0 = micros();
    machinePull();                        // the signal ("LED on")
    unsigned int guard = 0;
    while (digitalRead(KEY_PIN) == HIGH && ++guard < 20000) {}   // notice it
    digitalWrite(LED_PIN, HIGH);          // react
    unsigned long us = micros() - t0;
    interrupts();

    machineRelease();
    delay(200);
    digitalWrite(LED_PIN, LOW);

    if (guard >= 20000) {
      Serial.println(F("The wire came loose. Plug pin 2 into pin 12 again."));
      while (!machineWired()) delay(200);
      delay(500);
      continue;
    }

    if (us < 4) us = 4;                   // micros() counts in steps of 4
    Serial.print(F("Try "));
    Serial.print(tryNum);
    Serial.print(F(": "));
    Serial.print(us);
    Serial.println(F(" microseconds"));
    total += us;
    if (us < fastest) fastest = us;
    if (us > slowest) slowest = us;
    tryNum++;
  }

  float machineAvgUs = total / (float)TRIES;

  Serial.println();
  Serial.println(F("=============== RESULTS ==============="));
  Serial.print(F("You:     "));
  Serial.print(humanAvgMs, 0);
  Serial.println(F(" ms"));
  Serial.print(F("The Uno: "));
  Serial.print(machineAvgUs / 1000.0, 3);
  Serial.print(F(" ms  ("));
  Serial.print(machineAvgUs, 0);
  Serial.println(F(" microseconds)"));
  Serial.print(F("The Uno was about "));
  Serial.print((long)((humanAvgMs * 1000.0) / machineAvgUs));
  Serial.println(F(" times faster than you."));
  Serial.print(F("And it barely varies: all 5 tries were within "));
  Serial.print(slowest - fastest);
  Serial.println(F(" microseconds of each other."));
  Serial.println(F("Write your numbers on the board!"));
  Serial.println();
  Serial.println(F("To play again: unplug the jumper from pin 12."));

  while (machineWired()) delay(200);
  delay(500);
}

// ---------------- main ----------------

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(KEY_PIN, INPUT_PULLUP);
  machineRelease();
  Serial.begin(9600);
  randomSeed(analogRead(A0));             // A0 is unconnected: random noise

  Serial.println();
  Serial.println(F("######################################"));
  Serial.println(F("#        YOU vs. THE MACHINE         #"));
  Serial.println(F("######################################"));
}

void loop() {
  humanRound();
  machineRound();
}
