/*
  TWO-UNO STARTER TEMPLATE
  ------------------------
  Hardware: 2 Arduino Unos + male-male jumper wires. Nothing else.
  Upload this SAME sketch to both boards.

  Wiring between the boards (3 jumpers):
    Board A pin 7  -> Board B pin 8
    Board B pin 7  -> Board A pin 8
    Board A GND    -> Board B GND      (always needed!)
  Each board also gets its own key: a jumper in pin 2, tapped into GND.

  As-is, this sketch is already a telegraph: press your key and your
  partner's L LED lights up (and theirs lights yours).

  Project ideas:
    - Telegraph: add Morse decoding so the receiver sees letters.
    - Reaction duel: one board lights both LEDs at a random time;
      whoever's key goes down first wins.

  Safety: pin 7 never outputs 5V, it only pulls the wire to GND, so a
  wiring mix-up between the boards can't damage anything.

  Using Claude? Paste this file in and say: "I have two Arduino Unos and
  jumper wires only. Starting from this template, build a ..."
*/

const int LED_PIN = LED_BUILTIN;   // pin 13
const int KEY_PIN = 2;             // your key: tap into GND
const int OUT_PIN = 7;             // signal TO your partner (their pin 8)
const int IN_PIN  = 8;             // signal FROM your partner (their pin 7)

// True while your key jumper is touching GND.
bool keyDown() {
  return digitalRead(KEY_PIN) == LOW;
}

// Send a signal to your partner: on = pull the wire LOW, off = let go.
void sendSignal(bool on) {
  if (on) {
    digitalWrite(OUT_PIN, LOW);
    pinMode(OUT_PIN, OUTPUT);
  } else {
    pinMode(OUT_PIN, INPUT);
  }
}

// True while your partner is sending a signal.
bool partnerSignal() {
  return digitalRead(IN_PIN) == LOW;
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(KEY_PIN, INPUT_PULLUP);
  pinMode(IN_PIN, INPUT_PULLUP);     // reads "no signal" if unplugged
  sendSignal(false);
  Serial.begin(9600);
  Serial.println(F("Two-Uno template running. Press your key; your partner's LED should light."));
}

void loop() {
  // ---- Telegraph demo: replace everything below with your project ----
  sendSignal(keyDown());
  bool partner = partnerSignal();
  digitalWrite(LED_PIN, partner ? HIGH : LOW);

  static bool wasPartner = false;
  if (partner && !wasPartner) Serial.println(F("Partner pressed"));
  wasPartner = partner;
}
