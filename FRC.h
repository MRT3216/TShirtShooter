#ifndef FRC_h
#define FRC_h

#include "Arduino.h"

#define MINVAL 20 // decimal 20 corresponds with the pwm full reverse
#define MAXVAL 74 //decimal 74 corresponds with the pwm full forward
#define MIDDLE 47 // decimal 47 corresponds with the pwm midpoint

class Jaguar {
    byte pin, current;
  public:
    Jaguar(byte);
    void set(byte);
    void off();
};

class Spike { // our spikes only needed to go one way
    byte fpin;
    bool iopen;
  public:
    Spike(byte);
    void open();
    void close();
    bool isopen();
};

class Spike2way { // our spikes only needed to go one way
    byte fpin,rpin;
    byte state; // 0 = off, 1 = fwd, 2 = rev, 3 = brake
  public:
    Spike2way(byte,byte);
    void off();
    void fwd();
    void rev();
    void brake();
    byte getstate();
};

#endif
