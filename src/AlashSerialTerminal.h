/*
 *      Serial terminal library for Arduino
 *      Source:         https://github.com/Alash-electronics/AlashSerialTerminal
 */

#ifndef Alash-electronics_SERIAL_TERMINAL_H_
#define Alash-electronics_SERIAL_TERMINAL_H_

#include <Arduino.h>
#include <string.h>
#define ST_RX_BUFFER_SIZE       32
#define ST_NUM_COMMAND_CHARS    8
class SerialTerminal{
public:
    explicit SerialTerminal(char newlineChar='\n', char delimiterChar=' ');

    void addCommand(const char *command, void(*function)());
    void setDefaultHandler(void (*function)(const char *));

    void setSerialEcho(bool doEcho);
    void setPostCommandHandler(void (*function)(void));

    void readSerial();
    void clearBuffer();

    char *getNext();
    char *getRemaining();

private:
    struct SerialTerminalCallback {
        char command[ST_NUM_COMMAND_CHARS + 1];
        void (*function)();
    };

    SerialTerminalCallback *_commandList;
    byte _numCommands;
    char _newlineChar;
    char _delimiter[2];
    char _rxBuffer[ST_RX_BUFFER_SIZE + 1];
    byte _rxBufferIndex;
    char *_lastPos;

    bool doCharEcho;
    void (*_postCommandHandler)(void);

    void (*_defaultHandler)(const char *);
};

#endif // Alash-electronics_SERIAL_TERMINAL_H_
