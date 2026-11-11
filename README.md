# You vs. the Machine

Code for a 30-minute Arduino workshop on why we use microcontrollers, plus starter templates for projects to try afterward. Everything runs on an Arduino Uno (or a clone like the Elegoo Uno R3) with male-male jumper wires and nothing else.

## What's here

| Folder | What it is |
|---|---|
| `you_vs_machine_uno/` | The workshop activity. Race your Uno, then let it race itself. Ready to upload, no changes needed. |
| `one_uno_template/` | Starter for one-board projects: Morse messenger, conductivity tester, ghost detector. |
| `two_uno_template/` | Starter for two-board projects: telegraph, reaction duel. Upload it to both boards. |

## How to use

1. Click **Code → Download ZIP** above and unzip it.
2. Open the `.ino` file inside a folder with **Arduino IDE 2**.
3. Select **Tools → Board → Arduino Uno** and your board's port under **Tools → Port**.
4. Click **Upload** (the arrow), then open **Tools → Serial Monitor** at **9600 baud**.

Each sketch explains its wiring in the comments at the top.

## The workshop activity

- **Round 1 (you):** put a jumper in pin 2. When the L LED lights, tap the free end into a GND hole. 5 tries.
- **Round 2 (the machine):** move the free end into pin 12. The Uno detects it and times its own reaction in microseconds.

## Using Claude with the templates

Paste a template into Claude and describe what you want, for example:

> I have an Arduino Uno and jumper wires only. Starting from this template, build a Morse code messenger that blinks whatever I type in the Serial Monitor.

Upload and test after every change, so you know which change broke something if it does.
