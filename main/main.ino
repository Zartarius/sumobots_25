#include "Motor.h"
#include "Sensor.h"
#include <Arduino.h>

static constexpr float MAX_DIST = 120.0;
static constexpr float SLOW_DOWN_DIST = 30.0;
static constexpr int FORWARD_MAX_SPEED = 255;
static constexpr int FORWARD_MIN_SPEED = 128;
static constexpr int REVERSE_SPEED = 64;
static constexpr int PIVOT_SPEED = 100;
static constexpr unsigned long PIVOT_TIME = 250;
static constexpr unsigned long FULL_360_TIME = 10000; // ignored
static constexpr unsigned long MIN_GAP = 5;

Motor::Motor motor;
Sensor::USSensor us_sensor{4, 5};
// 1st trig 2nd echo
Sensor::USSensor us_sensor_left{7, 6};
Sensor::USSensor us_sensor_right{12, 11};
Sensor::IRSensor ir_sensor{2};

void setup(void) {
  Serial.begin(9600);
  Serial.println("Booting...");
  // Start after 5 seconds
  delay(5000);
  Serial.println("sybau");
}

void loop(void) {
  float distance;
  // Keep driving forward while opponent is in view and white tape isn't
  // detected

  while ((distance = us_sensor.get_distance()) < MAX_DIST/* &&
         !ir_sensor.get_signal()*/) {
    #ifdef DEBUG
    Serial.println("forward");
    #endif

    if (distance > SLOW_DOWN_DIST) {
      motor.drive_forward(FORWARD_MAX_SPEED);
    } else {
      float frac = distance / SLOW_DOWN_DIST;
      float speed = FORWARD_MAX_SPEED * frac * frac;
      motor.drive_forward((speed < FORWARD_MIN_SPEED) ? FORWARD_MIN_SPEED
                                                      : speed);
    }
  }

  /*
  // If robot is at the edge, drive in reverse, this will save it in MOST cases
  if (ir_sensor.get_signal()) {
    // do_for(motor, us_sensor, &Motor::Motor::drive_reverse, REVERSE_SPEED,
    // 500);
    Serial.println("reverse");
    const unsigned long epoch = millis();
    motor.drive_reverse(REVERSE_SPEED);
    delay(500);
    motor.brake();
  }*/

  unsigned long epoch = millis();
  float time_mul = 1.0;

  // Only run opponent detection while within timeout and in bounds
  while (/*!ir_sensor.get_signal() && */
         /*(millis() - epoch) < FULL_360_TIME && // timeout, could be removed */
             (distance = us_sensor.get_distance()) > MAX_DIST) {

    float left_distance = us_sensor_left.get_distance();
    float right_distance = us_sensor_right.get_distance();
    float dist_delta = left_distance - right_distance;

    #ifdef DEBUG
    Serial.print("searching because of dist: ");
    Serial.print(distance);
    Serial.print(" delta: ");
    Serial.print(left_distance);
    Serial.print(" - ");
    Serial.print(right_distance);
    Serial.print(" = ");
    Serial.println(dist_delta);
    #endif

    if ((left_distance > MAX_DIST &&
         right_distance > MAX_DIST) /*|| abs(dist_delta) < MIN_GAP*/) {
      #ifdef DEBUG
      Serial.println("couldnt find, pivot cycle");
      #endif
      motor.pivot_right(PIVOT_SPEED);
    } else if (dist_delta < 0) {
      #ifdef DEBUG
      Serial.println("left");
      #endif
      motor.pivot_right(PIVOT_SPEED);
    } else {
      #ifdef DEBUG
      Serial.println("right");
      #endif
      motor.pivot_left(PIVOT_SPEED);
    }

    // second chance
    float center_distance = us_sensor.get_distance();
    if (center_distance < DETECTION_THRESHOLD) break;

    delay(200);
    motor.brake();
    delay(200);
  }
}
