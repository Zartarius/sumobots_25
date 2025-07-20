#include <Arduino.h>

using namespace Motor, Sensor;

Motor motor;
USSensor us_sensor;
IRsensor ir_sensor;

static constexpr float MAX_DIST = 100.0;
static constexpr float FORWARD_SPEED = 69.0; // to change
static constexpr float REVRSE_SPEED = 69.0; // to change
static constexpr float PIVOT_SPEED = 69.0; // to change
static constexpr unsigned long PIVOT_TIME = 100; // to change

void setup(void) {
  Serial.begin(9600);
  us_sensor = USSensor(6,5);
}

void loop(void) {
    float distance;
    // Keep driving forward while opponent is in view and white tape isn't detected
    while ((distance = us_sensor.get_distance()) <= MAX_DIST && !ir_sensor.get_signal()) {
       motor.drive_forward(distance * distance + FORWARD_SPEED); 
    }
   
    motor.brake();

    // If robot is at the edge, drive in reverse, this will save it in MOST cases
    if (ir_sensor.get_signal()) {
        do_for(motor, us_sensor, &Motor::Motor::drive_reverse, REVERSE_SPEED, 500);
        motor.brake();
    }
    
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
          
    motor.brake();
}
