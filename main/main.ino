#include <Arduino.h>
#include "Motor.h"
#include "Sensor.h"

static constexpr float MAX_DIST = 10.0;
static constexpr float SLOW_DOWN_DIST = 30.0;
static constexpr int FORWARD_MAX_SPEED = 255; 
static constexpr int FORWARD_MIN_SPEED = 48; 
static constexpr int REVERSE_SPEED = 64; 
static constexpr int PIVOT_SPEED = 64; 
static constexpr unsigned long PIVOT_TIME = 150; 
static constexpr unsigned long FULL_360_TIME = 5000; //ignored
static constexpr unsigned long MIN_GAP = 5;


Motor::Motor motor;
Sensor::USSensor us_sensor{6, 5};
// 1st trig 2nd echo
Sensor::USSensor us_sensor_left{11, 12};
Sensor::USSensor us_sensor_right{4, 3};
//IRSensor ir_sensor;

void setup(void) {
    Serial.begin(9600);
    // Start after 5 seconds
    delay(5000); 
}

void loop(void) {
    float distance;
    // Keep driving forward while opponent is in view and white tape isn't detected
    while ((distance = us_sensor.get_distance()) <= MAX_DIST /* && 
            !ir_sensor.get_signal()*/) 
    {   
        Serial.println("forward rn");
        if (distance > SLOW_DOWN_DIST) {
            motor.drive_forward(FORWARD_MAX_SPEED);
        } else {
            float frac = distance / SLOW_DOWN_DIST;
            float speed = FORWARD_MAX_SPEED * frac * frac;
            motor.drive_forward((speed < FORWARD_MIN_SPEED) ? FORWARD_MIN_SPEED : speed); 
        }
    }
   
    //motor.brake();

    // If robot is at the edge, drive in reverse, this will save it in MOST cases
    /*if (ir_sensor.get_signal()) {
        // do_for(motor, us_sensor, &Motor::Motor::drive_reverse, REVERSE_SPEED, 500);
        const unsigned long epoch = millis();
        motor.drive_reverse(REVERSE_SPEED);
        while (millis() - epoch < 500) {}
        motor.brake();
    }*/

    unsigned long epoch = millis();
    unsigned long inner_epoch = millis();
    float time_mul = 1.0;

    // Only run opponent detection while within timeout and in bounds
    while (/*!ir_sensor.get_signal() && */
            /*(millis() - epoch) < FULL_360_TIME &&*/ // timeout, could be removed
            (distance = us_sensor.get_distance()) > MAX_DIST)
    {
        Serial.println("zhiger");

        int left_distance = us_sensor_left.get_distance();
        Serial.print("left");
        Serial.println(left_distance);
        int right_distance = us_sensor_right.get_distance();
        Serial.print("right");
        Serial.println(right_distance);
        int dist_delta = left_distance - right_distance;
        
        if ((left_distance > MAX_DIST && right_distance > MAX_DIST) || abs(dist_delta) < MIN_GAP ) {
            Serial.println("couldnt find, pivot cycle");
            motor.pivot_right(PIVOT_SPEED);
        } else if (dist_delta < 0) {
            Serial.println("left");
            motor.pivot_left(PIVOT_SPEED);
        } else {
            Serial.println("right");
            motor.pivot_right(PIVOT_SPEED);
        }
        delay(2000);
    }

    Serial.println("Outside");
          
    //motor.brake();
}


/*
unsigned long time = PIVOT_TIME;
// Opponent detecting logic
while (time < 3000) {
    if (do_for(motor, us_sensor, &Motor::Motor::pivot_right, PIVOT_SPEED, time)) {
        break;
    }
    motor.brake();
  
    if (do_for(motor, us_sensor, &Motor::Motor::pivot_left, PIVOT_SPEED, time + time)) {
        break;
    }
    motor.brake();

    if (do_for(motor, us_sensor, &Motor::Motor::pivot_right, PIVOT_SPEED, time)) {
        break;
    }
    motor.brake();

    time += PIVOT_TIME;
}
*/
