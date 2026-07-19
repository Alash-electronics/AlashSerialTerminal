# AlashSerialTerminal — Serial Terminal library for Arduino

> 🌐 **English** | [Русский](README_ru.md)

Built by Alash Education for educational use: a general-purpose serial terminal library for Arduino that parses ASCII commands and arguments coming in over a serial port.

![screenshot](https://github.com/Alash-electronics/AlashSerialTerminal/assets/171731850/15fae77c-308b-484e-bcaa-c4d08d2a475a)

## Hardware

Any Arduino-compatible board with a serial port, including:

Arduino:
* UNO
* Nano
* Micro
* Pro / Pro Mini
* Mega / Mega2560
* Leonardo

Other platforms:
* DUE
* ESP8266
* ESP32
* SAMD21
* STM32F1

## Usage

**Initialization**

Create a serial terminal object, optionally with a custom newline character and argument delimiter.

Default newline: `'\n'`
Default delimiter: `' '` (space)

```cpp
#include <AlashSerialTerminal.h>

char newlineChar = '\n';
char delimiterChar = ' ';

SerialTerminal term(newlineChar, delimiterChar);

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
}
```

**Registering commands**

Commands are registered at startup with a callback handler. Registration matches the command only — arguments are read separately. The callback fires once the full command, including the newline, has been received.

```cpp
void setup() {
  ...
  term.addCommand("?", cmdHelp);
  term.addCommand("help", cmdHelp);
  term.addCommand("on", cmdLedOn);
  term.addCommand("off", cmdLedOff);
}

void cmdHelp() {
  Serial.println(F("Serial terminal usage:"));
  Serial.println(F("  help or ?           Show this help"));
  Serial.println(F("  on                  Turn the LED on"));
  Serial.println(F("  off                 Turn the LED off"));
}

void cmdLedOn() {
  Serial.println(F("LED on"));
  digitalWrite(LED_BUILTIN, HIGH);
}

void cmdLedOff() {
  Serial.println(F("LED off"));
  digitalWrite(LED_BUILTIN, LOW);
}
```

**Setting a default handler**

Optional: fires when a command isn't recognized.

```cpp
void setup() {
  ...
  term.setDefaultHandler(unknownCommand);
}

void unknownCommand(const char *command) {
  Serial.print(F("Unknown command: "));
  Serial.println(command);
}
```

**Reading the serial port**

```cpp
void loop() {
  term.readSerial();
}
```

**Getting the next argument**

```cpp
char *arg = term.getNext();
if (arg != NULL) {
  Serial.print(F("Argument: "));
  Serial.println(arg);
} else {
  Serial.println(F("No argument"));
}
```

**Getting the remaining characters**

```cpp
char *arg = term.getRemaining();
if (arg != NULL) {
  Serial.print(F("Remaining: "));
  Serial.println(arg);
}
```

**Clearing the buffer**

```cpp
term.clearBuffer();
```

**Serial echo**

Optional: echoes typed characters back over serial — useful for terminal programs like PuTTY. Supports both Backspace and `^H`/`^127`.

```cpp
term.setSerialEcho(true);
```

**Post-command handler**

Optional: runs a function right after a command has been processed.

```cpp
void setup() {
  ...
  term.setPostCommandHandler(postCommandHandler);
}

void postCommandHandler() {
  Serial.print(F("> ")); // simple prompt
}
```

## Configuration

`SerialTerminal.h` exposes two configuration macros:

* `ST_RX_BUFFER_SIZE` — receive buffer size, default 32 bytes (command + arguments, excluding the terminating `'\0'`).
* `ST_NUM_COMMAND_CHARS` — max command length, default 8 bytes (excluding `'\0'`).

## Other Arduino libraries from Alash-electronics

[github.com/Alash-electronics](https://github.com/Alash-electronics)
