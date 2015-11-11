//setup sbus input
#include <SBUS.h>
//#include <PWM.h>
#include <avr/interrupt.h>
#include "FRC.h"

#define FAIL 20      // stops the motors at 20% frame loss (maybe not a good idea?)
#define FAILTIME 500 // stops the motors after 500ms of no data

#define FIRETIME 250 // ms open valve time for firing the cannon

//pin assignments
#define leftpin 10
#define rightpin 9
#define barrel1pin 5
#define barrel2pin 8
#define barrel3pin 7
#define liftuppin 3
#define liftdownpin 4

// set up output objects
Jaguar leftmotor(leftpin), rightmotor(rightpin);
Spike barrel1(barrel1pin), barrel2(barrel2pin), barrel3(barrel3pin);
Spike2way lift(liftuppin,liftdownpin);

SBUS sbus(Serial);

byte currenttube = 0;
bool firing = false;
long long firetimer;

// sbus output:
/// analog: -63 ~ 3 ~ 70
/// toggle: -48 ~ 3 ~ 55
/// channel 2 - right stick
/// channel 3 - left stick
/// channel 6 - left toggle
/// channel 7 - right toggle
/// channel 5 - lever thing (inverted)


void setup() {
  sbus.begin();

  //init pwm freq (seems to only work on the UNO
  TCCR1B = TCCR1B & 0b11111000 | 0x04;

  //set up the motors in off state
  leftmotor.off();
  rightmotor.off();
  lift.off();
  barrel1.close();
  barrel2.close();
  barrel3.close();

  //Serial.begin(115200);
/*
  InitTimersSafe();
  SetPinFrequencySafe(leftpin,122);
  SetPinFrequencySafe(rightpin,122);
*/
  pinMode(13,OUTPUT); // these are indicators
  pinMode(12,OUTPUT);
  
}

void loop() {
  // tank drive
  byte rightout = map(sbus.getNormalizedChannel(2),-63,70,20,74);
  byte leftout = 74 - map(sbus.getNormalizedChannel(3),-63,70,20,74) + 20;
  // barrel stuff
  int ltoggle = sbus.getNormalizedChannel(6);
  if (ltoggle < -23) {
    currenttube = 1;
  } else if (ltoggle > 27) {
     currenttube = 3;
  } else {
    currenttube = 2;
  }

  //firing = sbus.getNormalizedChannel(7) > 27;
  
  if (!firing && sbus.getNormalizedChannel(7) > 27) {
    firing = true;
    firetimer = millis();
  }
  if ((sbus.getNormalizedChannel(7) < 27)) {
    firing = false;
    firetimer = 0;
  }
  bool isfiring = firing && (firetimer + FIRETIME) > millis();
  
  // lift stuff
  int lever = sbus.getNormalizedChannel(5);
  
  
  if (sbus.getFailsafeStatus() == SBUS_FAILSAFE_ACTIVE || sbus.getFrameLoss() > FAIL || sbus.getFrameLoss() < 0 || millis() - sbus.getLastTime() > FAILTIME) {
    // this is the failsafe mode: disables motors (and pneumatics soon)
    leftmotor.off();
    rightmotor.off();
    lift.off();
    barrel1.close();
    barrel2.close();
    barrel3.close();
    digitalWrite(12,LOW);
    digitalWrite(13,millis()%500>250);
  } else {
    digitalWrite(12,isfiring);
    digitalWrite(13,HIGH);
    leftmotor.set(leftout);
    rightmotor.set(rightout);
    if (isfiring) {
      switch (currenttube) {
        case 1:
          barrel1.open();
          barrel2.close();
          barrel3.close();
        break;
        case 2:
          barrel2.open();
          barrel1.close();
          barrel3.close();
        break;
        case 3:
          barrel3.open();
          barrel1.close();
          barrel2.close();
        break;
        default:
          firing = false;
      }
    } else {
      barrel1.close();
      barrel2.close();
      barrel3.close();
    }
    if (lever > 35) {
      lift.rev();
    } else if (lever < -32) {
      lift.fwd();
    } else {
      lift.off();
    }
  }
  //delay(500);
}

ISR(TIMER2_COMPA_vect)
{
  sbus.process();
}
