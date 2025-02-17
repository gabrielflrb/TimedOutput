/*
 * TimedOutput Library
 * 
 * Copyright (c) 2025 Gabriel Bernardes
 *
 * Licensed under the MIT License. See LICENSE file in the project root for more details.
 */

#ifndef TIMEDOUTPUT_HPP
#define TIMEDOUTPUT_HPP

#define TIMEDOUTPUT_VERSION "1.0.0"

#include <Arduino.h>

class TimedOutput {
private:
    enum class Mode : uint8_t{ 
        OFF,        //Output off
        SINGLE,     //Output on for a set time, then off
        REPEAT,     //Output on/off, repeating until off() called or mode change
        REPEATX,    //Output on/off for a set number of cycles, then off
        ALWAYS_ON   //Output is on until call to off() or mode change
    };
    
    uint32_t _prevMillis = 0;
    uint32_t _durOn = 0;
    uint32_t _durOff = 0;
    uint8_t _counter = 0;
    uint8_t _pin = 0;
    Mode _mode = Mode::OFF;
    
    inline uint8_t _logicOn(){ return invertLogic ? LOW : HIGH; }
    inline uint8_t _logicOff(){ return invertLogic ? HIGH : LOW; }
    
    void _setOutput(bool on);
    
    void _configure(Mode m);
    void _configure(Mode m, uint32_t durOn) { _configure(m, durOn, _durOff); };
    void _configure(Mode m, uint32_t durOn, uint32_t durOff);
    void _configure(Mode m, uint32_t durOn, uint32_t durOff, uint8_t counter);

public:
    bool invertLogic = false;
    
    TimedOutput(uint8_t pin, bool invertLogic = false): _pin(pin), invertLogic(invertLogic) {}
    
    void init() { 
        pinMode(_pin,OUTPUT); 
        off(); 
    }
    
    uint32_t getDurOn() const { return _durOn; }
    void setDurOn(uint32_t durOn) { _durOn = durOn; }

    uint32_t getDurOff() const { return _durOff; }
    void setDurOff(uint32_t durOff) { _durOff = durOff; }

    uint32_t getCycleDuration() const { return _durOn + _durOff; }
    
    uint8_t getCounter() const { return _counter; }

    uint8_t getPin() const { return _pin; }

    bool currentOutput() { return digitalRead(_pin) == _logicOn(); }

    bool isOff(){ return _mode == Mode::OFF; }

    void on() { _configure(Mode::ALWAYS_ON);};
    void off();
        
    void single(uint32_t durOn) { _configure(Mode::SINGLE, durOn); }
    
    void repeat() { repeat(_durOn,_durOff); }
    void repeat(uint32_t durOn){ repeat(durOn, durOn); }
    void repeat(uint32_t durOn, uint32_t durOff){ _configure(Mode::REPEAT, durOn, durOff); }
    
    
    void repeatDutyCycle(double dutyCycle, uint32_t duration){ 
        repeat(
            static_cast<uint32_t>(duration * dutyCycle), 
            static_cast<uint32_t>(duration * (1.0 - dutyCycle))
        ); 
    }

    void repeatX(uint8_t times){ repeatX(times, _durOn, _durOff); }
    void repeatX(uint8_t times, uint32_t durOn){ repeatX(times,durOn,durOn); }
    void repeatX(uint8_t times, uint32_t durOn, uint32_t durOff){ _configure(Mode::REPEATX, durOn,durOff,times); }
    
    void repeatXDutyCycle(uint8_t times, double dutyCycle, uint32_t duration){ 
        repeatX(
            times,
            static_cast<uint32_t>(duration * dutyCycle), 
            static_cast<uint32_t>(duration * (1.0 - dutyCycle))
        ); 
    }

    void update();
};

#endif