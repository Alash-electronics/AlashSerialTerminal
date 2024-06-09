/*
 *      AlashSerialTerminal.cpp
 *      Serial terminal library for Arduino.
 *      Source:         https://github.com/Alash-electronics/AlashSerialTerminal
 */

#include "AlashSerialTerminal.h"
SerialTerminal::SerialTerminal(char newlineChar, char delimiterChar) :
        _commandList(NULL),
        _numCommands(0),
        _newlineChar(newlineChar),
        _lastPos(NULL),
        _defaultHandler(NULL)
{
    // strtok_r needs a null-terminated string
    _delimiter[0] = delimiterChar;
    _delimiter[1] = '\0';

    // Clear receive buffer
    clearBuffer();
}
void SerialTerminal::addCommand(const char *command, void (*function)()){
    // Increase size command list by one
    _commandList = (SerialTerminalCallback *)realloc(_commandList,
            sizeof(SerialTerminalCallback) * (_numCommands + 1));

    // Copy command and store command callback handler
    strncpy(_commandList[_numCommands].command, command, ST_NUM_COMMAND_CHARS);
    _commandList[_numCommands].function = function;

    // Increment number of commands
    _numCommands++;
}
void SerialTerminal::setSerialEcho(bool doEcho){
    doCharEcho = doEcho;
}
void SerialTerminal::setPostCommandHandler(void (*function)(void)){
    _postCommandHandler = function;
}
void SerialTerminal::setDefaultHandler(void (*function)(const char *)){
    _defaultHandler = function;
}
void SerialTerminal::readSerial(){
    bool matched = false;
    char *command;
    char c;

    while (Serial.available() > 0) {
        // Read one character from serial port
        c = (char)Serial.read();

        // Check newline character \c '\\r' or \c '\\n' at the end of a command
        if (c == _newlineChar) {
            //Echo received char
            if (doCharEcho) {
                Serial.println();
            }

            // Search for command at start of buffer
            command = strtok_r(_rxBuffer, _delimiter, &_lastPos);

            if (command != NULL) {
                for (int i = 0; i < _numCommands; i++) {
                    // Compare the found command against the list of known commands for a match
                    if (strncmp(command, _commandList[i].command, ST_NUM_COMMAND_CHARS) == 0) {
                        // Call command callback handler
                        (*_commandList[i].function)();
                        matched = true;
                        break;
                    }
                }

                if (!matched && (_defaultHandler != NULL)) {
                    (*_defaultHandler)(command);
                }
            }

            //Run the post command handler.
            if (_postCommandHandler) {
                (*_postCommandHandler)();
            }

            clearBuffer();
        // either ^H og 127 backspace chars
        } else if (c == '\b' || c == 127) {
            if (_rxBufferIndex > 0) {
                _rxBufferIndex--;
                _rxBuffer[_rxBufferIndex] = '\0';
                if (doCharEcho) {
                    Serial.print("\b \b"); // 1 char back, space, 1 char back
                }
            }
        } else if (isprint(c)) {
            // Store printable characters in serial receive buffer
            if (_rxBufferIndex < ST_RX_BUFFER_SIZE) {
                _rxBuffer[_rxBufferIndex++] = c;
                _rxBuffer[_rxBufferIndex] = '\0';
                //Echo received char
                if (doCharEcho) {
                    Serial.print(c);
                }
            }
        }
    }
}
void SerialTerminal::clearBuffer(){
    _rxBuffer[0] = '\0';
    _rxBufferIndex = 0;
}
char *SerialTerminal::getNext(){
    return strtok_r(NULL, _delimiter, &_lastPos);
}
char *SerialTerminal::getRemaining(){
    return strtok_r(NULL, "", &_lastPos);
}
