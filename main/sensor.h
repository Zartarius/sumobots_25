#pragma once
#include <Arduino.h>

namespace Sensor {
class USSensor {
  private:
    int trig_pin_;
    int echo_pin_;
  public:
    explicit USSensor(int trig_pin, int echo_pin) : trig_pin_{trig_pin}, echo_pin_{echo_pin} {
      pinMode(trig_pin_, OUTPUT);
      pinMode(echo_pin_, INPUT);
      digitalWrite(trig_pin_, LOW);
    }
    USSensor& operator=(USSensor&& s) {
        trig_pin_ = s.trig_pin_;
        echo_pin_ = s.echo_pin_;
        return *this;
    }

    float get_distance() {
      float dist = 99999, time;
      
      digitalWrite(trig_pin_, HIGH);
      delayMicroseconds(10);
      digitalWrite(trig_pin_, LOW);
      
      time = pulseIn(echo_pin_, HIGH);
      // distance formula
      dist = time * 0.017;
      dist = dist == 0 ? 99999 : dist;

      #ifdef DEBUG
      Serial.print("Time: ");
      Serial.print(time);
      Serial.print(" Dist: ");
      Serial.println(dist);
      #endif

      
      delay(250);
      
      return dist;
    }
};

class IRSensor {
  private:
    int signal_pin_;
  public:
    IRSensor(void) = default;
    IRSensor(int signal_pin) : signal_pin_{signal_pin} {
      pinMode(signal_pin_, INPUT);
    }

    bool get_signal() {
      return digitalRead(signal_pin_);
    }
};
}