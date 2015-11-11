#include "FRC.h"
#include <Arduino.h>
//#include <PWM.h>


////////////////////////////////////////////// jaguar

Jaguar::Jaguar(byte p) {
  pin = p;
  pinMode(pin,OUTPUT);
}

void Jaguar::set(byte s) {
  current = s;
  current = constrain(current,MINVAL,MAXVAL);
  analogWrite(pin,current);
}

void Jaguar::off() {
  current = MIDDLE;
  analogWrite(pin,current);
}

///////////////////////////////////////////////// spike

Spike::Spike(byte p) {
  fpin = p;
  pinMode(fpin,OUTPUT);
}

void Spike::open() {
  digitalWrite(fpin,HIGH);
  iopen = true;
}

void Spike::close() {
  digitalWrite(fpin,LOW);
  iopen = false;
}

bool Spike::isopen() {
  return iopen;
}

//////////////////////////////////////////////// 2 way spike


Spike2way::Spike2way(byte p, byte r) {
  fpin = p;
  rpin = r;
  pinMode(fpin,OUTPUT);
  pinMode(rpin,OUTPUT);
}

void Spike2way::off() {
  digitalWrite(fpin,LOW);
  digitalWrite(rpin,LOW);
  state = 0;
}

void Spike2way::fwd() {
  digitalWrite(fpin,HIGH);
  digitalWrite(rpin,LOW);
  state = 1;
}
void Spike2way::rev() {
  digitalWrite(fpin,LOW);
  digitalWrite(rpin,HIGH);
  state = 2;
}
void Spike2way::brake() {
  digitalWrite(fpin,HIGH);
  digitalWrite(rpin,HIGH);
  state = 3;
}

byte Spike2way::getstate() {
  return state;
}
