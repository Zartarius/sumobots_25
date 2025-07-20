#include <Arduino.h>

using namespace Motor, Sensor;

Motor motor;
USSensor us_sensor;
IRsensor ir_sensor;

static constexpr float MAX_DIST = 100.0;
static constexpr float FORWARD_SPEED = 69.0; // to change
static constexpr float REVRSE_SPEED = 69.0; // to change
static constexpr unsigned long PIVOT_TIME = 100; // to change
static constexpr int PIVOT_LEFT = 0;
static constexpr int PIVOT_RIGHT = 1;

void setup(void) {
  Serial.begin(9600);
  us_sensor = USSensor(6,5);
}

void loop(void) {
    float distance;
    while ((distance = us_sensor.get_distance()) <= MAX_DIST /*&& IRsensor doesn't detect tape*/) {
       motor.drive_forward(FORWARD_SPEED - distance); // Increase speed as it gets closer?
    }
   
    motor.brake();
    
    // if (IR sensor detects tape) {
    //    motor.drive_reverse(REVERSE_SPEED);
    // }
    unsigned long time = PIVOT_TIME;

    while (time < 3000) {
        // motor.pivot_right for time
        // motor.brake()
        // motor.pivot_left for 2 * time
        // motor.brake()
        // motor.pivot_right for time
        // motor.brake()

        time += PIVOT_TIME;
    }

    motor.brake();
}
