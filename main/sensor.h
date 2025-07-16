#pragma once
#include <Arduino.h>

namespace Sensor {

class USSensor {
  private:
    const int trig_pin_;
    const int echo_pin_;
  public:
    explicit USSensor(int trig_pin, int echo_pin) : trig_pin_{trig_pin}, echo_pin_{echo_pin} {
      pinMode(trig_pin_, OUTPUT);
      pinMode(echo_pin_, INPUT);
    }
    USSensor(const USSensor&& s) : trig_pin_{s.trig_pin_}, echo_pin_{s.echo_pin_}  {}
    USSensor& USSensor::operator=(USSensor&& s) {
        trig_pin_ = s.trig_pin_;
        echo_pin_ = s.echo_pin_;
        return *this;
    }

    float get_distance() {
      digitalWrite(trig_pin_, LOW);
      // wait to clear out
      delayMicroseconds(2);
      digitalWrite(trig_pin_, HIGH);
      delayMicroseconds(10);
      digitalWrite(trig_pin_, LOW);
      
      float time = pulseIn(echo_pin_, HIGH);
      // distance formula
      return (time * 0.0343) / 2;
    }
};

class IRSensor {
  private:
  public:
};


}
