/*
 * TimedOutput Library
 * 
 * Copyright (c) 2025 Gabriel Bernardes
 *
 * Licensed under the MIT License. See LICENSE file in the project root for more details.
 */

#include "TimedOutput.hpp"

void TimedOutput::_setOutput(bool output){
    digitalWrite(_pin, output ? _logicOn() : _logicOff());
}

void TimedOutput::_configure(Mode m){
    _mode = m;
    _setOutput(true);
}

void TimedOutput::_configure(Mode m, uint32_t durOn, uint32_t durOff){
    if (durOn == 0)
        return;
        
    _durOn = durOn;
    _durOff = durOff;
    _prevMillis = millis();

    _configure(m);
}

void TimedOutput::_configure(Mode m, uint32_t durOn, uint32_t durOff, uint8_t counter){
    // Invalid configuration; return
    if (m == Mode::REPEATX && counter == 0)
        return;
    
    _counter = counter;
     
    _configure(m,durOn, durOff);
}

void TimedOutput::off() {
    _mode = Mode::OFF;
    _setOutput(false);
}

void TimedOutput::update() {
    if (_mode == Mode::OFF || _mode == Mode::ALWAYS_ON)
        return;
    
    uint32_t currentMillis = millis();
    uint32_t millisDelta = currentMillis - _prevMillis;
    
    bool currOutput;

    switch (_mode) {
        case Mode::SINGLE:
            if (millisDelta >= _durOn)
                off();
            break;
        case Mode::REPEAT:
        case Mode::REPEATX:
            currOutput = currentOutput();
            // If the current output is on 
            // and the duration has passed
            if ((currOutput && millisDelta >= _durOn)
            || (!currOutput && millisDelta >= _durOff)) {
                //Toggle output
                _setOutput(!currOutput); 
                _prevMillis = currentMillis;    
                if (_mode == Mode::REPEATX && currOutput) {
                    if (--_counter == 0)
                        off();
                }
            }
            break;
        default:
            break;
    }
}