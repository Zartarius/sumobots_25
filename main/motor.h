#pragma once
#include <Arduino.h>


namespace Motor {
// Peform the specified member function for a given amount of time
// Return true if the sensor detects anything -> This may be 
// redundant depending on where this function is used.
bool do_for(Motor& motor, 
            Sensor::USsensor& sensor,  
            void (Motor::Motor::*action)(const int),
            const int speed,
            const unsigned long ms_delay) {
  
  (motor.*action)(speed);     
  const unsigned long start = millis();
  float distance;
  while ((distance = sensor.get_distance()) > MAX_DIST && (millis() - start) < ms_delay) {}
            
  return distance <= MAX_DIST;
}

class Motor {
   private:
    // todo: put values here for pins
    // motor 1 - assumed left
    static constexpr int PIN_IN1 = 1;
    static constexpr int PIN_IN2 = 2;

    // motor 2 - assumed right
    static constexpr int PIN_IN3 = 3;
    static constexpr int PIN_IN4 = 4;

    static constexpr int PIN_ENA = 5;
    static constexpr int PIN_ENB = 6;

    void forward_left() {
        digitalWrite(PIN_IN1, HIGH);
        digitalWrite(PIN_IN2, LOW);
    }

    void set_speed_left(int speed) {
        analogWrite(PIN_ENA, speed);
    }

    void forward_right() {
        digitalWrite(PIN_IN3, HIGH);
        digitalWrite(PIN_IN4, LOW);
    }

    void set_speed_right(int speed) {
        analogWrite(PIN_ENB, speed);
    }

    void reverse_left() {
        digitalWrite(PIN_IN1, LOW);
        digitalWrite(PIN_IN2, HIGH);
    }

    void reverse_right() {
        digitalWrite(PIN_IN3, LOW);
        digitalWrite(PIN_IN4, HIGH);
    }

    void brake_left() {
        digitalWrite(PIN_IN1, HIGH);
        digitalWrite(PIN_IN2, HIGH);
    }

    void brake_right() {
        digitalWrite(PIN_IN3, HIGH);
        digitalWrite(PIN_IN4, HIGH);
    }

   public:
    //        ~Motor() = default;
    Motor(void) {
        pinMode(PIN_IN1, OUTPUT);
        pinMode(PIN_IN2, OUTPUT);
        pinMode(PIN_IN3, OUTPUT);
        pinMode(PIN_IN4, OUTPUT);
        pinMode(PIN_ENA, OUTPUT);
        pinMode(PIN_ENB, OUTPUT);
    }

    void drive_forward(const int speed) {
        set_speed_left(speed);
        set_speed_right(speed);
        forward_left();
        forward_right();
    }

    void drive_reverse(const int speed) {
        set_speed_left(speed);
        set_speed_right(speed);
        reverse_left();
        reverse_right();
    }

    void pivot_left(const int speed) {
        set_speed_left(speed);
        set_speed_right(speed);
        forward_left();
        reverse_right();
    }

    void pivot_right(const int speed) {
        set_speed_left(speed);
        set_speed_right(speed);
        forward_right();
        reverse_left();
    }
    
    void brake() {
        brake_left();
        brake_right();
    }
};
}
