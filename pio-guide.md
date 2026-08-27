# PlatformIO + Zed — Quick Study Notes

---

## 1. Mental Model

```text
Zed
 ↓
PlatformIO CLI (pio)
 ↓
Arduino framework
 ↓
Board/toolchain
 ↓
Microcontroller
```

For an Uno:

```text
Zed
 ↓
PlatformIO
 ↓
Arduino framework
 ↓
AVR-GCC
 ↓
ATmega328P
```

For ESP32:

```text
Zed
 ↓
PlatformIO
 ↓
Arduino framework
 ↓
ESP32 toolchain
 ↓
ESP32
```

You write normal C++.

---

# 2. Basic Commands

## Check PlatformIO

```bash
pio --version
```

---

## See available commands

```bash
pio --help
```

For help on a specific command:

```bash
pio run --help
```

---

# 3. Creating a Project

Go somewhere you keep your projects:

```bash
cd ~/Documents
```

Create a project for Arduino Uno:

```bash
pio project init --board uno
```

Or ESP32:

```bash
pio project init --board esp32dev
```

Then open the project in Zed:

```bash
zed .
```

---

# 4. Project Structure

A typical PlatformIO project looks like:

```text
my-project/
├── platformio.ini
├── src/
│   └── main.cpp
├── include/
├── lib/
├── test/
├── .pio/
└── compile_commands.json
```

### Important folders

### `src/`

Your main source code.

```text
src/main.cpp
```

### `include/`

Your own header files.

Example:

```text
include/display.h
```

### `lib/`

Libraries that belong specifically to your project.

### `test/`

Unit tests.

### `.pio/`

PlatformIO's generated build files.

**Don't manually edit this.**

### `platformio.ini`

The most important configuration file.

---

# 5. `platformio.ini`

For an Arduino Uno:

```ini
[env:uno]
platform = atmelavr
board = uno
framework = arduino
```

This tells PlatformIO:

```text
Board     → Arduino Uno
Platform  → AVR
Framework → Arduino
```

---

# 6. Writing Arduino Code

Your code goes in:

```text
src/main.cpp
```

Example:

```cpp
#include <Arduino.h>

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);

    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
}
```

Because you're using:

```ini
framework = arduino
```

you still have all the normal Arduino functions:

```cpp
setup()
loop()

pinMode()
digitalWrite()
digitalRead()

analogRead()
analogWrite()

delay()
millis()

Serial.begin()
Serial.print()
Serial.println()
```

---

# 7. Build Your Project

From the project root:

```bash
pio run
```

PlatformIO will:

1. Read `platformio.ini`
2. Find the board
3. Find the framework
4. Find libraries
5. Compile your code
6. Link everything
7. Produce the firmware

Successful build:

```text
SUCCESS
```

---

# 8. Upload to the Board

Plug your board into your Mac.

Check detected devices:

```bash
pio device list
```

Then:

```bash
pio run -t upload
```

PlatformIO will build and upload the firmware.

You usually don't need to manually specify the port.

---

# 9. Serial Monitor

Start a serial monitor:

```bash
pio device monitor
```

Specify baud rate:

```bash
pio device monitor -b 115200
```

Your Arduino code:

```cpp
Serial.begin(115200);

Serial.println("Hello!");
```

Then:

```bash
pio device monitor -b 115200
```

---

# 10. The Main Workflow

Most of the time you'll repeat:

```bash
pio run
```

Build.

```bash
pio run -t upload
```

Upload.

```bash
pio device monitor -b 115200
```

Monitor serial output.

So:

```text
WRITE CODE
   ↓
pio run
   ↓
BUILD
   ↓
pio run -t upload
   ↓
BOARD
   ↓
pio device monitor
   ↓
DEBUG
```

---

# 11. Libraries

PlatformIO has its own library manager.

Search for a library:

```bash
pio pkg search "U8g2"
```

You can also browse the PlatformIO Library Registry.

Your project dependencies go inside:

```ini
lib_deps =
```

Example:

```ini
[env:uno]
platform = atmelavr
board = uno
framework = arduino

lib_deps =
    olikraus/U8g2
```

Then:

```bash
pio run
```

PlatformIO downloads the library automatically.

---

# 12. U8g2 Example

`platformio.ini`:

```ini
lib_deps =
    olikraus/U8g2
```

Then:

```cpp
#include <Arduino.h>
#include <U8g2lib.h>
```

You don't manually download U8g2.

PlatformIO manages it.

---

# 13. Multiple Libraries

Example:

```ini
lib_deps =
    olikraus/U8g2
    arduino-libraries/Servo
```

Then your code can use:

```cpp
#include <U8g2lib.h>
#include <Servo.h>
```

---

# 14. See Installed Packages

```bash
pio pkg list
```

You can see things like:

```text
framework-arduino-avr
toolchain-atmelavr
```

and project libraries.

---

# 15. Updating Libraries

Check outdated packages:

```bash
pio pkg outdated
```

Update packages:

```bash
pio pkg update
```

Be careful with updates on an important project.

A library update can sometimes break existing code.

---

# 16. Removing Libraries

Remove a dependency from:

```ini
lib_deps =
```

Then run:

```bash
pio run
```

PlatformIO will no longer use that dependency for the project.

---

# 17. Finding Boards

Search boards:

```bash
pio boards
```

Search for a particular board:

```bash
pio boards uno
```

Example:

```bash
pio boards esp32
```

The important identifier is the **board ID**.

For Uno:

```text
uno
```

For many ESP32 DevKit boards:

```text
esp32dev
```

---

# 18. Different Boards in One Project

You can have multiple environments:

```ini
[env:uno]
platform = atmelavr
board = uno
framework = arduino

[env:esp32]
platform = espressif32
board = esp32dev
framework = arduino
```

Build a specific environment:

```bash
pio run -e uno
```

or:

```bash
pio run -e esp32
```

Upload a specific environment:

```bash
pio run -e uno -t upload
```

---

# 19. Useful Build Commands

Clean the build:

```bash
pio run -t clean
```

Build:

```bash
pio run
```

Build + upload:

```bash
pio run -t upload
```

Build a specific environment:

```bash
pio run -e uno
```

Clean and rebuild:

```bash
pio run -t clean
pio run
```

---

# 20. Compilation Database + Zed

For Zed/clangd to understand PlatformIO projects:

```bash
pio run -t compiledb
```

This creates:

```text
compile_commands.json
```

It gives clangd information about:

- compiler
- include paths
- defines
- board
- architecture
- compiler flags

---



# 22. Project Configuration vs Code

Keep hardware/project configuration in:

```text
platformio.ini
```

Keep C++ code in:

```text
src/main.cpp
```

For example:

```text
my-oled-project/
├── platformio.ini
└── src/
    └── main.cpp
```

`platformio.ini`:

```ini
[env:uno]
platform = atmelavr
board = uno
framework = arduino

lib_deps =
    olikraus/U8g2
```

`main.cpp`:

```cpp
#include <Arduino.h>
#include <U8g2lib.h>
```

---

# 23. Common Errors

## `pio: command not found`

PlatformIO isn't available in your shell PATH.

Check:

```bash
pio --version
```

---

## `Arduino.h file not found`

Usually a Zed/clangd configuration problem rather than a PlatformIO problem.

First run:

```bash
pio run
```

If that works, regenerate:

```bash
pio run -t compiledb
```

Then restart Zed.

---


## Upload fails

Check:

```bash
pio device list
```

Then try:

```bash
pio run -t upload
```

If necessary, explicitly specify the port:

```bash
pio run -t upload --upload-port /dev/cu.usbserial-XXXX
```

---

# 24. Serial Debugging

Example:

```cpp
void setup() {
    Serial.begin(115200);
}

void loop() {
    Serial.println("Running...");
    delay(1000);
}
```

Then:

```bash
pio device monitor -b 115200
```

This is one of your most useful debugging tools.

---

# 25. Environment Variables / Configuration

You can override settings from the command line.

For example:

```bash
pio run -e uno
```

means:

> Build only the `uno` environment.

This becomes useful when projects get bigger.

---

# 26. Git

PlatformIO projects work normally with Git.

Generally commit:

```text
platformio.ini
src/
include/
lib/
test/
```

Don't commit:

```text
.pio/
```

Add this to `.gitignore`:

```gitignore
.pio/
```

You generally want your project configuration and source code in Git, not generated build artifacts.

---

# 27. The Commands Worth Memorizing

You don't need to memorize everything.

These are the **core 10**:

```bash
pio --version
```

Check PIO.

```bash
pio project init --board uno
```

Create project.

```bash
pio run
```

Build.

```bash
pio run -t upload
```

Upload.

```bash
pio device list
```

Find connected boards.

```bash
pio device monitor -b 115200
```

Serial monitor.

```bash
pio pkg search "U8g2"
```

Search libraries.

```bash
pio pkg list
```

See packages.

```bash
pio run -t compiledb
```

Generate clangd compilation database.

```bash
pio --help
```

When you forget something.

---

# 28. Your Typical Project

For something like your OLED Tetris:

```text
tetris/
├── .clangd
├── .gitignore
├── platformio.ini
├── compile_commands.json
├── src/
│   └── main.cpp
├── include/
└── lib/
```

`platformio.ini`:

```ini
[env:uno]
platform = atmelavr
board = uno
framework = arduino

lib_deps =
    olikraus/U8g2
```

Then:

```bash
zed .
```

Write code.

```bash
pio run
```

Build.

```bash
pio run -t upload
```

Upload.

```bash
pio device monitor -b 115200
```

Debug.

---

# 🧠 The Big Picture

Don't think:

> "I need to learn PlatformIO."

Think:

> **"PlatformIO is my embedded build system."**

You mainly need to understand:

```text
platformio.ini
      ↓
board + framework + libraries
      ↓
     pio
      ↓
compiler
      ↓
firmware
      ↓
microcontroller
```

And your day-to-day life is basically:

```bash
pio run
pio run -t upload
pio device monitor
```

Everything else is something you can look up when you need it.

## Recommended learning order

Since you're learning Arduino at the same time:

1. **GPIO** — `pinMode`, `digitalRead`, `digitalWrite`
2. **Serial** — `Serial.begin`, `print`, `println`
3. **Analog** — `analogRead`, PWM
4. **Timing** — `delay`, then `millis`
5. **Buttons + debouncing**
6. **Libraries**
7. **I²C**
8. **SPI**
9. **OLEDs**
10. **Sensors**
11. **Servos / motors**
12. **Interrupts**
13. **Timers**
14. **AVR registers / bare-metal programming**
