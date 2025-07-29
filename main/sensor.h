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
    }
    //USSensor(const USSensor&& s) : trig_pin_{s.trig_pin_}, echo_pin_{s.echo_pin_}  {}
    USSensor& operator=(USSensor&& s) {
        trig_pin_ = s.trig_pin_;
        echo_pin_ = s.echo_pin_;
        return *this;
    }

    float get_distance() {
      float dist = 99999;

      while (dist == 99999) {
        digitalWrite(trig_pin_, LOW);
        // wait to clear out
        delayMicroseconds(5);
        digitalWrite(trig_pin_, HIGH);
        delayMicroseconds(12);
        digitalWrite(trig_pin_, LOW);
        delayMicroseconds(5); 
        
        float time = pulseIn(echo_pin_, HIGH);
        // distance formula
        dist = time * 0.01715;
        dist = dist == 0 ? 99999 : dist;
      }
      //Serial.println(dist);
      
      return dist;// (time * 0.0343) / 2;
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
